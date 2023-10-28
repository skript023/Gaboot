#include "product_service.hpp"
#include "util/exception.hpp"

namespace gaboot
{
	HttpResponsePtr product_service::findAll(HttpRequestPtr const& req)
	{
		Json::Value json;
		
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

			json["message"] = "Success retreive products data";
			json["success"] = true;
			json["data"] = res;
			json["lastPage"] = lastPage;

			auto response = HttpResponse::newHttpJsonResponse(json);

			return response;
		}
		catch (const DrogonDbException& e)
		{
			json["message"] = fmt::format("Unable retrieve products data, error caught on {}", e.base().what());
			json["success"] = false;

			auto response = HttpResponse::newHttpJsonResponse(json);
			response->setStatusCode(k500InternalServerError);

			return response;
		}
	}
	HttpResponsePtr product_service::create(HttpRequestPtr const& req)
	{
		std::string error;
		Json::Value json;

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

			if (!schema.validate(product.toJson(), error))
			{
				return BadRequestException(error).response();
			}

			db().insert(product);

			json["message"] = "Create product success";
			json["success"] = true;

			auto response = HttpResponse::newHttpJsonResponse(json);

			return response;
		}
		catch (const DrogonDbException& e)
		{
			LOG(WARNING) << e.base().what();

			json["message"] = e.base().what();
			json["success"] = false;

			auto response = HttpResponse::newHttpJsonResponse(json);
			response->setStatusCode(HttpStatusCode::k500InternalServerError);

			return response;
		}
	}
	HttpResponsePtr product_service::findOne(HttpRequestPtr const& req, std::string&& id)
	{
		Json::Value json_cb;

		try
		{
			if (id.empty() || !util::is_numeric(id))
			{
				return BadRequestException("Parameters requirement doesn't match").response();
			}

			const auto product = db().findFutureByPrimaryKey(stoll(id)).get();

			if (!product.getId()) return NotFoundException("Product data is empty 0 data found").response();

			json_cb["message"] = "Success retrieve products data";
			json_cb["success"] = true;
			json_cb["data"] = product.toJson();

			auto response = HttpResponse::newHttpJsonResponse(json_cb);

			return response;
		}
		catch (const DrogonDbException& e)
		{
			json_cb["message"] = fmt::format("Unable retrieve products data, error caught on {}", e.base().what());
			json_cb["success"] = false;

			auto response = HttpResponse::newHttpJsonResponse(json_cb);
			response->setStatusCode(k500InternalServerError);

			return response;
		}
	}
	HttpResponsePtr product_service::update(HttpRequestPtr const& req, std::string&& id)
	{
		Json::Value resp;

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

			resp["data"] = product.toJson();
			resp["message"] = "Success update customer data.";
			resp["success"] = true;

			auto response = HttpResponse::newHttpJsonResponse(resp);
			return response;
		}
		catch (const DrogonDbException& e)
		{
			resp["message"] = fmt::format("Unable to update data, error caught on {}", e.base().what());
			resp["success"] = false;

			auto response = HttpResponse::newHttpJsonResponse(resp);

			LOG(WARNING) << e.base().what();

			return response;
		}
	}
	HttpResponsePtr product_service::remove(HttpRequestPtr const& req, std::string&& id)
	{
		Json::Value json;

		try
		{
			if (id.empty() || !util::is_numeric(id))
			{
				return BadRequestException("Parameters requirement doesn't match").response();
			}

			const auto record = db().deleteFutureByPrimaryKey(stoll(id)).get();

			if (record != 0)
			{
				json["message"] = fmt::format("Delete user on {} successfully", record);
				json["success"] = true;

				auto response = HttpResponse::newHttpJsonResponse(json);
				return response;
			}
			
			return NotFoundException("Unable to delete non-existing data").response();
		}
		catch (const DrogonDbException& e)
		{
			json["message"] = fmt::format("Failed delete user, error caught on {}", e.base().what());
			json["success"] = false;

			auto response = HttpResponse::newHttpJsonResponse(json);
			response->setStatusCode(k500InternalServerError);

			return response;
		}
	}
}