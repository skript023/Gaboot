#include "category_service.hpp"

#include <exception/exception.hpp>
#include <uploader/upload.hpp>

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
		TRY_CLAUSE
		{
			MultiPartParser fileUpload;

			if (fileUpload.parse(req) != 0 || fileUpload.getFiles().size() == 0)
			{
				throw BadRequestException("Requirement doesn't match");
			}

			auto& file = fileUpload.getFiles()[0];

			if (!util::multipart_tojson(fileUpload, m_data)) throw BadRequestException("Unknown error");

			validator schema({
				{"name", "type:string|required|alphanum"},
				{"description", "type:string|required|alphanum"}
			});

			Categories category(m_data);

			upload_file upload(&file, *category.getName(), "categories");

			category.setCreatedAt(trantor::Date::now());
			category.setUpdatedAt(trantor::Date::now());
			category.setImagePath(upload.get_image_path());
			category.setThumbnailPath(upload.get_thumbnail_path());

			if (!schema.validate(category.toJson(), m_error))
			{
				throw BadRequestException(m_error);
			}

			db().insert(category);

			m_cache_category.clear();

			if (!upload.save()) throw BadRequestException("Unable to save image");

			m_response.m_message = "Create category success";
			m_response.m_success = true;

			auto response = HttpResponse::newHttpJsonResponse(m_response.to_json());
			response->setStatusCode(k201Created);

			return response;
		} EXCEPT_CLAUSE
    }
    HttpResponsePtr category_service::findAll(HttpRequestPtr const& req)
    {
		TRY_CLAUSE
		{
			auto& limitParam = req->getParameter("limit");
			auto& pageParam = req->getParameter("page");

			const size_t limit = limitParam.empty() && !util::is_numeric(limitParam) ? 10 : stoull(limitParam);
			const size_t page = pageParam.empty() && !util::is_numeric(pageParam) ? 0 : stoull(pageParam) - 1;

			this->load_cache();

			const auto categories = m_cache_category.limit(limit).offset(page * limit).find_all();

			if (categories.empty())
			{
				m_response.m_message = "0 category found";
				m_response.m_success = true;

				return HttpResponse::newHttpJsonResponse(m_response.to_json());
			}

			Json::Value data(Json::arrayValue);


			const size_t lastPage = (categories.size() / (limit + (categories.size() % limit))) == 0 ? 0 : 1;

			m_response.m_message = "Success retreive categories data";
			m_response.m_data = categories;
			m_response.m_success = true;
			m_response.m_last_page = lastPage;

			return HttpResponse::newHttpJsonResponse(m_response.to_json());
		} EXCEPT_CLAUSE
    }
	HttpResponsePtr category_service::findOne(HttpRequestPtr const& req, std::string&& id)
	{
		TRY_CLAUSE
		{
			if (id.empty())
			{
				throw BadRequestException("Requirement doesn't match");
			}

			this->load_cache();

			auto category = m_cache_category.find(id);

			if (!category) throw NotFoundException("Unable retrieve category detail");

			m_response.m_message = "Success retrieve category data";
			m_response.m_success = true;
			m_response.m_data = category;

			return HttpResponse::newHttpJsonResponse(m_response.to_json());
		} EXCEPT_CLAUSE
	}
	HttpResponsePtr category_service::update(HttpRequestPtr const& req, std::string&& id)
	{
		TRY_CLAUSE
		{
			MultiPartParser multipart;

			this->load_cache();

			if (id.empty())
			{
				throw BadRequestException("Parameters requirement doesn't match");
			}

			if (multipart.parse(req) != 0)
			{
				throw BadRequestException("Requirement doesn't match");
			}

			auto& file = multipart.getFiles()[0];

			util::multipart_tojson(multipart, m_data);

			auto category = m_cache_category.find(id);
			if (!category) throw NotFoundException("Unable to update non-existing data");

			category->updateByJson(m_data);
			category->setId(id);
			category->setUpdatedAt(trantor::Date::now());

			validator schema({
				{"name", "type:string|required|alphanum"},
				{"description", "type:string|required|alphanum"}
			});

			if (!schema.validate(category->toJson(), m_error))
			{
				throw BadRequestException(m_error);
			}

			upload_file upload(&file, *category->getName(), "categories");

			if (multipart.getFiles().size() > 0 && util::allowed_image(file.getFileExtension().data()))
			{
				category->setImagePath(upload.get_image_path());
				category->setThumbnailPath(upload.get_thumbnail_path());
			}

			if (!m_cache_category.update(id, *category))
				throw CustomException<k500InternalServerError>("Unable to update non-existing cache");

			if (!db().updateFuture(*category).get())
				throw CustomException<k500InternalServerError>("Unable to update non-existing data");

			if (multipart.getFiles().size() > 0 && util::allowed_image(file.getFileExtension().data()))
			{
				LOG(INFO) << "File saved at " << upload.get_image_path();

				upload.save();
			}

			m_response.m_message = "Success update category data.";
			m_response.m_success = true;

			return HttpResponse::newHttpJsonResponse(m_response.to_json());
		} EXCEPT_CLAUSE
	}
	HttpResponsePtr category_service::remove(HttpRequestPtr const& req, std::string&& id)
	{
		TRY_CLAUSE
		{
			if (id.empty())
			{
				throw BadRequestException("Parameters requirement doesn't match");
			}

			this->load_cache();

			const auto record = db().deleteFutureByPrimaryKey(id).get();

			if (!record)
			{
				throw NotFoundException("Unable to delete non-existing data");
			}

			m_cache_category.remove(id);

			m_response.m_message = fmt::format("Delete category on {} successfully", record);
			m_response.m_success = true;

			return HttpResponse::newHttpJsonResponse(m_response.to_json());
		} EXCEPT_CLAUSE
	}
	HttpResponsePtr category_service::getImage(HttpRequestPtr const& req, std::string&& id)
	{
		TRY_CLAUSE
		{
			Categories category;

			if (id.empty())
			{
				LOG(WARNING) << "ID is empty or ID is not numeric";

				throw BadRequestException("Parameters requirement doesn't match");
			}

			this->load_cache();

			if (!m_cache_category.find(id, &category))
			{
				std::filesystem::path file(*category.getImagePath());

				if (!std::filesystem::exists(file))
				{
					LOG(WARNING) << "File at " << file.lexically_normal() << " doesn't exist in server";

					m_response.m_message = "Unable to retreive category picture, please upload your category picture";
					m_response.m_success = false;

					auto response = HttpResponse::newHttpJsonResponse(m_response.to_json());
					response->setStatusCode(k404NotFound);

					return response;
				}

				if (auto image = category.getImagePath(); image && !image->empty())
					return HttpResponse::newFileResponse(*category.getImagePath());
			}

			throw NotFoundException("Unable to retreive category image");
		} EXCEPT_CLAUSE
	}
	HttpResponsePtr category_service::getThumbnail(HttpRequestPtr const& req, std::string&& id)
	{
		TRY_CLAUSE
		{
			Categories category;

			if (id.empty())
			{
				LOG(WARNING) << "ID is empty or ID is not numeric";

				throw BadRequestException("Parameters requirement doesn't match");
			}

			this->load_cache();

			if (m_cache_category.find(id, &category))
			{
				std::filesystem::path file(*category.getThumbnailPath());

				if (!std::filesystem::exists(file))
				{
					LOG(WARNING) << "File at " << file.lexically_normal() << " doesn't exist in server";

					m_response.m_message = "Unable to retreive category picture, please upload your category picture";
					m_response.m_success = false;

					auto response = HttpResponse::newHttpJsonResponse(m_response.to_json());
					response->setStatusCode(k404NotFound);

					return response;
				}

				if (auto image = category.getImagePath(); image && !image->empty())
					return HttpResponse::newFileResponse(*category.getThumbnailPath());
			}

			throw NotFoundException("Unable to retreive category image");
		} EXCEPT_CLAUSE
	}
}