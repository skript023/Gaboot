#include "product_service.hpp"
#include "util/exception.hpp"

namespace gaboot
{
	HttpResponsePtr product_service::findAll(HttpRequestPtr const& req)
	{
		try
		{
			auto& limitParam = req->getParameter("limit");
			auto& pageParam = req->getParameter("page");

			const size_t limit = limitParam.empty() && !util::is_numeric(limitParam) ? 10 : stoull(limitParam);
			const size_t page = pageParam.empty() && !util::is_numeric(pageParam) ? 0 : stoull(pageParam) - 1;

			const auto products = db().orderBy(MasterProducts::Cols::_name).limit(limit).offset(page * limit).findFutureAll().get();

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
	HttpResponsePtr product_service::create(HttpRequestPtr const& req)
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

			MasterProducts product(*data);

			if (!schema.validate(product.toJson(), m_error))
			{
				return BadRequestException(m_error).response();
			}

			db().insert(product);

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
	HttpResponsePtr product_service::findOne(HttpRequestPtr const& req, std::string&& id)
	{
		try
		{
			if (id.empty() || !util::is_numeric(id))
			{
				return BadRequestException("Parameters requirement doesn't match").response();
			}

			const auto product = db().findFutureByPrimaryKey(stoll(id)).get();

			if (!product.getId()) return NotFoundException("Product data is empty 0 data found").response();

			m_response.m_message = "Success retrieve products data";
			m_response.m_success = true;
			m_response.m_data = product.toJson();

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
	HttpResponsePtr product_service::update(HttpRequestPtr const& req, std::string&& id)
	{
		try
		{
			const auto& json = req->getJsonObject();

			if (!json) return BadRequestException().response();

			if (id.empty() || !util::is_numeric(id))
			{
				return BadRequestException("Parameters requirement doesn't match").response();
			}

			MasterProducts product(*json);
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
	HttpResponsePtr product_service::remove(HttpRequestPtr const& req, std::string&& id)
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