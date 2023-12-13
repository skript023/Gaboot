#include "category_service.hpp"

#include <util/exception.hpp>
#include <util/upload.hpp>

namespace gaboot
{
	category_service::category_service()
	{
		this->load_cache();
	}
	category_service::~category_service()
	{
		m_cache_category.clear();
	}
	HttpResponsePtr category_service::create(HttpRequestPtr const& req)
    {
		try
		{
			MultiPartParser fileUpload;

			if (fileUpload.parse(req) != 0 || fileUpload.getFiles().size() == 0)
			{
				return BadRequestException("Requirement doesn't match").response();
			}

			auto& file = fileUpload.getFiles()[0];

			if (!util::multipart_tojson(fileUpload, m_data)) return BadRequestException("Unknown error").response();

			validator schema({
				{"name", "type:string|required|alphanum"},
				{"description", "type:string|required|alphanum"}
			});

			Categories category(m_data);

			upload_file upload(&file, *category.getName(), "categories");

			category.setCreatedat(trantor::Date::now());
			category.setUpdatedat(trantor::Date::now());
			category.setImgpath(upload.get_image_path());
			category.setImgthumbpath(upload.get_thumbnail_path());

			if (!schema.validate(category.toJson(), m_error))
			{
				return BadRequestException(m_error).response();
			}
			
			db().insert(category);

			m_cache_category.clear();

			if (!upload.save()) return BadRequestException("Unable to save image").response();

			m_response.m_message = "Create category success";
			m_response.m_success = true;

			auto response = HttpResponse::newHttpJsonResponse(m_response.to_json());
			response->setStatusCode(k201Created);

			return response;
		}
		catch (const DrogonDbException& e)
		{
			LOG(WARNING) << e.base().what();

			m_response.m_message = e.base().what();
			m_response.m_success = false;

			auto response = HttpResponse::newHttpJsonResponse(m_response.to_json());
			response->setStatusCode(HttpStatusCode::k500InternalServerError);

			return response;
		}
    }
    HttpResponsePtr category_service::findAll(HttpRequestPtr const& req)
    {
		try
		{
			auto& limitParam = req->getParameter("limit");
			auto& pageParam = req->getParameter("page");

			const size_t limit = limitParam.empty() && !util::is_numeric(limitParam) ? 10 : stoull(limitParam);
			const size_t page = pageParam.empty() && !util::is_numeric(pageParam) ? 0 : stoull(pageParam) - 1;

			this->load_cache();

			const auto categories = m_cache_category.limit(limit).offset(page * limit).find_all();

			if (categories.empty())
			{
				m_response.m_message = "No data retrieved";
				m_response.m_success = false;

				return HttpResponse::newHttpJsonResponse(m_response.to_json());
			}

			Json::Value data(Json::arrayValue);

			std::ranges::for_each(categories.begin(), categories.end(), [&data](Categories const& category) {
				data.append(category.toJson());
			});

			const size_t lastPage = (categories.size() / (limit + (categories.size() % limit))) == 0 ? 0 : 1;

			m_response.m_message = "Success retreive categories data";
			m_response.m_success = true;
			m_response.m_data = data;
			m_response.m_last_page = lastPage;

			return HttpResponse::newHttpJsonResponse(m_response.to_json());
		}
		catch (const std::exception& e)
		{
			std::string error = fmt::format("Unable retrieve categories data, error caught on {}", e.what());

			return CustomException<k500InternalServerError>(error).response();
		}
    }
	HttpResponsePtr category_service::findOne(HttpRequestPtr const& req, std::string&& id)
	{
		try
		{
			if (id.empty() || !util::is_numeric(id))
			{
				return BadRequestException("Requirement doesn't match").response();
			}

			this->load_cache();

			const auto user = m_cache_category.find(stoll(id));

			if (!user) return NotFoundException("Unable retrieve category detail").response();

			m_response.m_message = "Success retrieve category data";
			m_response.m_success = true;
			m_response.m_data = user->toJson();

			return HttpResponse::newHttpJsonResponse(m_response.to_json());
		}
		catch (const std::exception& e)
		{
			std::string error = fmt::format("Cannot retrieve category data, error caught on {}", e.what());

			return CustomException<k500InternalServerError>(error).response();
		}
	}
	HttpResponsePtr category_service::update(HttpRequestPtr const& req, std::string&& id)
	{
		try
		{
			MultiPartParser multipart;

			this->load_cache();

			if (id.empty() || !util::is_numeric(id))
			{
				return BadRequestException("Parameters requirement doesn't match").response();
			}

			if (multipart.parse(req) != 0)
			{
				return BadRequestException("Requirement doesn't match").response();
			}

			auto& file = multipart.getFiles()[0];

			util::multipart_tojson(multipart, m_data);

			auto category = m_cache_category.find(stoll(id));
			if (!category) return NotFoundException("Unable to update non-existing data").response();

			category->updateByJson(m_data);
			category->setId(stoll(id));
			category->setUpdatedat(trantor::Date::now());

			validator schema({
				{"name", "type:string|required|alphanum"},
				{"description", "type:string|required|alphanum"}
			});

			if (!schema.validate(category->toJson(), m_error))
			{
				return BadRequestException(m_error).response();
			}

			upload_file upload(&file, *category->getName(), "categories");

			if (multipart.getFiles().size() > 0 && util::allowed_image(file.getFileExtension().data()))
			{
				category->setImgpath(upload.get_image_path());
				category->setImgthumbpath(upload.get_thumbnail_path());
			}

			if (!m_cache_category.update(stoll(id), *category))
				return CustomException<k500InternalServerError>("Unable to update non-existing cache").response();

			if (!db().updateFuture(*category).get())
				return CustomException<k500InternalServerError>("Unable to update non-existing data").response();

			if (multipart.getFiles().size() > 0 && util::allowed_image(file.getFileExtension().data()))
			{
				LOG(INFO) << "File saved at " << upload.get_image_path();
				
				upload.save();
			}

			m_response.m_message = "Success update category data.";
			m_response.m_success = true;

			auto response = HttpResponse::newHttpJsonResponse(m_response.to_json());
			return response;
		}
		catch (const std::exception& e)
		{
			LOG(WARNING) << e.what();

			m_response.m_message = fmt::format("Unable to update data, error caught on {}", e.what());
			m_response.m_success = false;

			auto response = HttpResponse::newHttpJsonResponse(m_response.to_json());
			response->setStatusCode(k500InternalServerError);

			return response;
		}
	}
	HttpResponsePtr category_service::remove(HttpRequestPtr const& req, std::string&& id)
	{
		try
		{
			if (id.empty() || !util::is_numeric(id))
			{
				return BadRequestException("Parameters requirement doesn't match").response();
			}

			this->load_cache();

			const auto record = db().deleteFutureByPrimaryKey(stoll(id)).get();

			if (record != 0)
			{
				m_cache_category.remove(stoll(id));

				m_response.m_message = fmt::format("Delete category on {} successfully", record);
				m_response.m_success = true;

				auto response = HttpResponse::newHttpJsonResponse(m_response.to_json());
				return response;
			}

			return NotFoundException("Unable to delete non-existing data").response();
		}
		catch (const DrogonDbException& e)
		{
			m_response.m_message = fmt::format("Failed delete category, error caught on {}", e.base().what());
			m_response.m_success = false;

			auto response = HttpResponse::newHttpJsonResponse(m_response.to_json());
			response->setStatusCode(k500InternalServerError);

			return response;
		}
	}
	HttpResponsePtr category_service::getImage(HttpRequestPtr const& req, std::string&& id)
	{
		Categories category;

		if (id.empty() || !util::is_numeric(id))
		{
			LOG(WARNING) << "ID is empty or ID is not numeric";

			return BadRequestException("Parameters requirement doesn't match").response();
		}

		this->load_cache();

		if (m_cache_category.find(stoll(id), &category))
		{
			std::filesystem::path file(*category.getImgpath());

			if (!std::filesystem::exists(file))
			{
				LOG(WARNING) << "File at " << file.lexically_normal() << " doesn't exist in server";

				m_response.m_message = "Unable to retreive category picture, please upload your category picture";
				m_response.m_success = false;

				auto response = HttpResponse::newHttpJsonResponse(m_response.to_json());
				response->setStatusCode(k404NotFound);

				return response;
			}

			if (auto image = category.getImgpath(); image && !image->empty())
				return HttpResponse::newFileResponse(*category.getImgpath());
		}

		m_response.m_message = "Unable to retreive category image";
		m_response.m_success = false;

		auto response = HttpResponse::newHttpJsonResponse(m_response.to_json());
		response->setStatusCode(k404NotFound);

		LOG(WARNING) << "Unable to retreive category image";

		return response;
	}
	HttpResponsePtr category_service::getThumbnail(HttpRequestPtr const& req, std::string&& id)
	{
		Categories category;

		if (id.empty() || !util::is_numeric(id))
		{
			LOG(WARNING) << "ID is empty or ID is not numeric";

			return BadRequestException("Parameters requirement doesn't match").response();
		}

		this->load_cache();

		if (m_cache_category.find(stoll(id), &category))
		{
			std::filesystem::path file(*category.getImgthumbpath());

			if (!std::filesystem::exists(file))
			{
				LOG(WARNING) << "File at " << file.lexically_normal() << " doesn't exist in server";

				m_response.m_message = "Unable to retreive category picture, please upload your category picture";
				m_response.m_success = false;

				auto response = HttpResponse::newHttpJsonResponse(m_response.to_json());
				response->setStatusCode(k404NotFound);

				return response;
			}

			if (auto image = category.getImgpath(); image && !image->empty())
				return HttpResponse::newFileResponse(*category.getImgthumbpath());
		}

		m_response.m_message = "Unable to retreive category image";
		m_response.m_success = false;

		auto response = HttpResponse::newHttpJsonResponse(m_response.to_json());
		response->setStatusCode(k404NotFound);

		LOG(WARNING) << "Unable to retreive category image";

		return response;
	}
}