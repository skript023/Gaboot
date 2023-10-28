#include "category_service.hpp"
#include <util/exception.hpp>
#include <util/upload.hpp>

namespace gaboot
{
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
				{"name", "type:string|required|minLength:3|alphabetOnly"},
				{"description", "type:string|required|minLength:3|alphabetOnly"}
			});

			Categories category(m_data);

			upload_file upload(file, category.getValueOfName(), "categories");

			category.setCreatedat(trantor::Date::now());
			category.setUpdatedat(trantor::Date::now());
			category.setImagepath(upload.get_image_path());
			category.setThumbnailpath(upload.get_thumbnail_path());

			if (!schema.validate(category.toJson(), m_error))
			{
				return BadRequestException(m_error).response();
			}
			
			db().insert(category);

			m_response.m_message = "Create product success";
			m_response.m_success = true;

			auto response = HttpResponse::newHttpJsonResponse(m_response.to_json());

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

			const auto products = db().orderBy(Categories::Cols::_name).limit(limit).offset(page * limit).findFutureAll().get();

			if (products.empty())
			{
				return NotFoundException("No data retrieved").response();
			}

			Json::Value res(Json::arrayValue);

			for (const auto& user : products)
			{
				res.append(user.toJson());
			}

			const size_t lastPage = (products.size() / (limit + (products.size() % limit))) == 0 ? 0 : 1;

			m_response.m_message = "Success retreive products data";
			m_response.m_success = true;
			m_response.m_data = res;
			m_response.m_last_page = lastPage;

			return HttpResponse::newHttpJsonResponse(m_response.to_json());
		}
		catch (const DrogonDbException& e)
		{
			m_response.m_message = fmt::format("Unable retrieve products data, error caught on {}", e.base().what());
			m_response.m_success = false;

			auto response = HttpResponse::newHttpJsonResponse(m_response.to_json());
			response->setStatusCode(k500InternalServerError);

			return response;
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

			const auto user = db().findFutureByPrimaryKey(stoll(id)).get();

			if (!user.getId()) return NotFoundException("Unable retrieve customer detail").response();

			m_response.m_message = "Success retrieve user data";
			m_response.m_success = true;
			m_response.m_data = user.toJson();

			return HttpResponse::newHttpJsonResponse(m_response.to_json());
		}
		catch (const DrogonDbException& e)
		{
			m_response.m_message = fmt::format("Cannot retrieve user data, error caught on {}", e.base().what());
			m_response.m_success = false;

			auto response = HttpResponse::newHttpJsonResponse(m_response.to_json());
			response->setStatusCode(k500InternalServerError);

			return response;
		}
	}
	HttpResponsePtr category_service::update(HttpRequestPtr const& req, std::string&& id)
	{
		try
		{
			if (id.empty() || !util::is_numeric(id))
			{
				return BadRequestException("Parameters requirement doesn't match").response();
			}

			MultiPartParser fileUpload;

			if (fileUpload.parse(req) != 0 || fileUpload.getFiles().size() == 0)
			{
				return BadRequestException("Requirement doesn't match").response();
			}

			auto& file = fileUpload.getFiles()[0];

			if (!util::multipart_tojson(fileUpload, m_data)) return BadRequestException("Unknown error").response();

			Categories product(m_data);
			product.setId(stoll(id));
			product.setUpdatedat(trantor::Date::now());

			const auto record = db().updateFuture(product).get();

			if (!record)
			{
				return NotFoundException("Unable to update non-existing product").response();
			}

			m_response.m_data = product.toJson();
			m_response.m_message = "Success update customer data.";
			m_response.m_success = true;

			auto response = HttpResponse::newHttpJsonResponse(m_response.to_json());
			return response;
		}
		catch (const DrogonDbException& e)
		{
			m_response.m_message = fmt::format("Unable to update data, error caught on {}", e.base().what());
			m_response.m_success = false;

			auto response = HttpResponse::newHttpJsonResponse(m_response.to_json());

			LOG(WARNING) << e.base().what();

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

			const auto record = db().deleteFutureByPrimaryKey(stoll(id)).get();

			if (record != 0)
			{
				m_response.m_message = fmt::format("Delete user on {} successfully", record);
				m_response.m_success = true;

				auto response = HttpResponse::newHttpJsonResponse(m_response.to_json());
				return response;
			}

			return NotFoundException("Unable to delete non-existing data").response();
		}
		catch (const DrogonDbException& e)
		{
			m_response.m_message = fmt::format("Failed delete user, error caught on {}", e.base().what());
			m_response.m_success = false;

			auto response = HttpResponse::newHttpJsonResponse(m_response.to_json());
			response->setStatusCode(k500InternalServerError);

			return response;
		}
	}
}