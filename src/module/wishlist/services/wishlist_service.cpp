#include "wishlist_service.hpp"
#include <util/exception.hpp>

namespace gaboot
{
    HttpResponsePtr wishlist_service::findAll(HttpRequestPtr const& req)
    {
		try
		{
			auto& limitParam = req->getParameter("limit");
			auto& pageParam = req->getParameter("page");

			const size_t limit = limitParam.empty() && !util::is_numeric(limitParam) ? 10 : stoull(limitParam);
			const size_t page = pageParam.empty() && !util::is_numeric(pageParam) ? 0 : stoull(pageParam) - 1;

			const auto products = db().orderBy(Wishlists::Cols::_category).limit(limit).offset(page * limit).findFutureAll().get();

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

			auto response = HttpResponse::newHttpJsonResponse(m_response.to_json());

			return response;
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
	HttpResponsePtr wishlist_service::create(HttpRequestPtr const& req)
	{
		try
		{
			const auto& data = req->getJsonObject();

			if (!data) return BadRequestException().response();

			validator schema({
				{"name", "type:string|required|minLength:3|alphabetOnly"},
				{"description", "type:string|required|minLength:3|alphabetOnly"},
				{"price", "type:number|required|numberOnly"},
				{"stock", "type:number|required|numberOnly"}
			});

			Wishlists product(*data);

			if (!schema.validate(product.toJson(), m_error))
			{
				return BadRequestException(m_error).response();
			}

			db().insert(product);

			m_json["message"] = "Create product success";
			m_json["success"] = true;

			auto response = HttpResponse::newHttpJsonResponse(m_json);

			return response;
		}
		catch (const DrogonDbException& e)
		{
			LOG(WARNING) << e.base().what();

			m_json["message"] = e.base().what();
			m_json["success"] = false;

			auto response = HttpResponse::newHttpJsonResponse(m_json);
			response->setStatusCode(HttpStatusCode::k500InternalServerError);

			return response;
		}
	}
	HttpResponsePtr wishlist_service::findOne(HttpRequestPtr const& req, std::string&& id)
	{
		try
		{
			if (id.empty() || !util::is_numeric(id))
			{
				return BadRequestException("Requirement doesn't match").response();
			}

			const auto user = db().findByPrimaryKey(stoll(id));

			if (!user.getId()) return NotFoundException("Unable retrieve wishlist detail").response();

			m_response.m_message = "Success retrieve wishlist data";
			m_response.m_success = true;
			m_response.m_data = user.toJson();

			return HttpResponse::newHttpJsonResponse(m_response.to_json());
		}
		catch (const DrogonDbException& e)
		{
			m_response.m_message = fmt::format("Cannot retrieve wishlist data, error caught on {}", e.base().what());
			m_response.m_success = false;

			auto response = HttpResponse::newHttpJsonResponse(m_response.to_json());
			response->setStatusCode(k500InternalServerError);

			return response;
		}
	}
	HttpResponsePtr wishlist_service::update(HttpRequestPtr const& req, std::string&& id)
	{
		try
		{
			const auto& json = req->getJsonObject();

			if (!json) return BadRequestException().response();

			if (id.empty() || !util::is_numeric(id))
			{
				return BadRequestException("Parameters requirement doesn't match").response();
			}

			Wishlists product(*json);
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
	HttpResponsePtr wishlist_service::remove(HttpRequestPtr const& req, std::string&& id)
	{
		if (id.empty() || !util::is_numeric(id))
		{
			return BadRequestException("Parameters requirement doesn't match").response();
		}

		try
		{
			const auto record = db().deleteByPrimaryKey(stoll(id));
			if (record != 0)
			{
				m_response.m_message = fmt::format("Delete user on {} successfully", record);
				m_response.m_success = true;

				return HttpResponse::newHttpJsonResponse(m_response.to_json());
			}

			return NotFoundException("Record not found").response();
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