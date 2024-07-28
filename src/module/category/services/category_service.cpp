#include "category_service.hpp"

#include <exception/exception.hpp>
#include <uploader/upload.hpp>

namespace gaboot
{
	category_service::category_service():
		m_database(std::make_unique<Mapper<Categories>>(DATABASE_CLIENT)),
		m_response(std::make_unique<response_data<CategoryResponse>>())
	{
		LOG(INFO) << "Category service registered.";
	}
	category_service::~category_service() noexcept
	{
		m_database.reset();
		m_response.reset();
	}
	HttpResponsePtr category_service::create(HttpRequestPtr const& req)
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

		m_database->insert(category);

		m_cache_category.clear();

		if (!upload.save()) throw BadRequestException("Unable to save image");

		m_response->m_message = "Create category success";
		m_response->m_success = true;

		return m_response->status(k201Created)->json();
    }
    HttpResponsePtr category_service::findAll(HttpRequestPtr const& req)
    {
		auto& limitParam = req->getParameter("limit");
		auto& pageParam = req->getParameter("page");

		const size_t limit = limitParam.empty() && !util::is_numeric(limitParam) ? 10 : stoull(limitParam);
		const size_t page = pageParam.empty() && !util::is_numeric(pageParam) ? 0 : stoull(pageParam) - 1;

		const auto categories = m_database->limit(limit).offset(page * limit).findAll();

		if (categories.empty())
		{
			m_response->m_message = "0 category found";
			m_response->m_success = true;

			return m_response->json();
		}

		Json::Value data(Json::arrayValue);


		const size_t lastPage = (categories.size() / (limit + (categories.size() % limit))) == 0 ? 0 : 1;

		m_response->m_message = "Success retreive categories data";
		m_response->m_data = categories;
		m_response->m_success = true;
		m_response->m_last_page = lastPage;

		return m_response->json();
    }
	HttpResponsePtr category_service::findOne(HttpRequestPtr const& req, std::string&& id)
	{
		if (id.empty())
		{
			throw BadRequestException("Requirement doesn't match");
		}

		this->load_cache();

		auto category = m_database->findByPrimaryKey(id);

		m_response->m_message = "Success retrieve category data";
		m_response->m_success = true;
		m_response->m_data = category;

		return m_response->json();
	}
	HttpResponsePtr category_service::update(HttpRequestPtr const& req, std::string&& id)
	{
		MultiPartParser multipart;

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

		if (!m_database->update(*category))
			throw CustomException<k500InternalServerError>("Unable to update non-existing data");

		if (multipart.getFiles().size() > 0 && util::allowed_image(file.getFileExtension().data()))
		{
			LOG(INFO) << "File saved at " << upload.get_image_path();

			upload.save();
		}

		m_response->m_message = "Success update category data.";
		m_response->m_success = true;

		return m_response->json();
	}
	HttpResponsePtr category_service::remove(HttpRequestPtr const& req, std::string&& id)
	{
		if (id.empty())
		{
			throw BadRequestException("Parameters requirement doesn't match");
		}

		if (!m_database->deleteByPrimaryKey(id))
		{
			throw NotFoundException("Unable to delete non-existing data");
		}

		m_cache_category.remove(id);

		m_response->m_message = fmt::format("Delete category successfully");
		m_response->m_success = true;

		return m_response->json();
	}
	HttpResponsePtr category_service::getImage(HttpRequestPtr const& req, std::string&& id)
	{
		if (id.empty())
		{
			LOG(WARNING) << "ID is empty or ID is not numeric";

			throw BadRequestException("Parameters requirement doesn't match");
		}

		Categories category = m_database->findByPrimaryKey(id);

		std::filesystem::path file(*category.getImagePath());

		if (!std::filesystem::exists(file))
		{
			LOG(WARNING) << "File at " << file.lexically_normal() << " doesn't exist in server";

			throw NotFoundException("Unable to retreive category picture, please upload your category picture");
		}

		return HttpResponse::newFileResponse(*category.getImagePath());
	}
	HttpResponsePtr category_service::getThumbnail(HttpRequestPtr const& req, std::string&& id)
	{
		if (id.empty())
		{
			LOG(WARNING) << "ID is empty or ID is not numeric";

			throw BadRequestException("Parameters requirement doesn't match");
		}

		Categories category = m_database->findByPrimaryKey(id);

		std::filesystem::path file(*category.getThumbnailPath());

		if (!std::filesystem::exists(file))
		{
			LOG(WARNING) << "File at " << file.lexically_normal() << " doesn't exist in server";

			throw NotFoundException("Unable to retreive category picture, please upload your category picture");
		}

		return HttpResponse::newFileResponse(*category.getThumbnailPath());
	}
}