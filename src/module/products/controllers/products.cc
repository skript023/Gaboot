#include "products.h"
#include <util/upload.hpp>
#include <util/exception.hpp>

// Add definition of your processing function here
namespace gaboot
{
	void products::findAll(HttpRequestPtr const& req, response_t&& callback)
	{
		auto& limitParam = req->getParameter("limit");
		auto& pageParam = req->getParameter("page");

		const size_t limit = limitParam.empty() && !util::is_numeric(limitParam) ? 10 : stoull(limitParam);
		const size_t page = pageParam.empty() && !util::is_numeric(pageParam) ? 0 : stoull(pageParam) - 1;
		
		db().orderBy(MasterProducts::Cols::_name).limit(limit).offset(page * limit).findAll([=](std::vector<MasterProducts> products)
		{
            Json::Value json;

			if (products.empty())
			{
				return callback(NotFoundException("No data retrieved").response());
			}

			Json::Value res(Json::arrayValue);

			for (const auto& user : products)
			{
				res.append(user.toJson());
			}

			const size_t lastPage = products.size() / limit + (products.size() % limit) == 0 ? 0 : 1;

			json["message"] = "Success retreive products data";
			json["success"] = true;
			json["data"] = res;
			json["lastPage"] = lastPage;

			auto response = HttpResponse::newHttpJsonResponse(json);

			return callback(response);
		}, [=](DrogonDbException const& e)
		{
			Json::Value json;

			json["message"] = fmt::format("Unable retrieve user data, error caught on {}", e.base().what());
			json["success"] = false;
			json["data"] = Json::arrayValue;

			auto response = HttpResponse::newHttpJsonResponse(json);
			response->setStatusCode(k500InternalServerError);

			return callback(response);
		});
	}
	void products::findOne(HttpRequestPtr const& req, response_t&& callback, std::string&& id)
	{
		if (id.empty() || !util::is_numeric(id))
		{
			return callback(BadRequestException("Parameters requirement doesn't match").response());
		}

		db().findByPrimaryKey(stoll(id), [=](MasterProducts product)
		{
			if (!product.getId()) return callback(NotFoundException("Unable retrieve product detail").response());

			Json::Value json_cb;
			json_cb["message"] = "Success retrieve user data";
			json_cb["success"] = true;
			json_cb["data"] = product.toJson();

			auto response = HttpResponse::newHttpJsonResponse(json_cb);

			return callback(response);
		}, [=](DrogonDbException const& e) 
		{
			Json::Value json_cb;
			json_cb["message"] = fmt::format("Unable retrieve user data, error caught on {}", e.base().what());
			json_cb["success"] = false;

			auto response = HttpResponse::newHttpJsonResponse(json_cb);
			response->setStatusCode(k500InternalServerError);

			return callback(response);
		});
	}
	void products::update(HttpRequestPtr const& req, response_t&& callback, std::string&& id)
	{
		const auto& json = req->getJsonObject();

		if (!json) return callback(BadRequestException().response());

		if (id.empty() || !util::is_numeric(id))
		{
			return callback(BadRequestException("Parameters requirement doesn't match").response());
		}

		MasterProducts product(*json);
		product.setId(stoll(id));
		product.setUpdatedat(trantor::Date::now());

		db().update(product, [=](size_t record)
		{
			Json::Value resp;

			if (!record) 
			{
				return callback(NotFoundException("Unable to update non-existing product").response());
			}

			resp["data"] = product.toJson();
			resp["message"] = "Success update customer data.";
			resp["success"] = true;

			auto response = HttpResponse::newHttpJsonResponse(resp);
			return callback(response);
		}, [=](DrogonDbException const& e)
		{
			Json::Value resp;

			resp["message"] = fmt::format("Unable to update data, error caught on {}", e.base().what());
            resp["success"] = false;

            auto response = HttpResponse::newHttpJsonResponse(resp);

            LOG(WARNING) << e.base().what();

            return callback(response);
		});
	}
	void products::create(HttpRequestPtr const& req, response_t&& callback)
	{
		std::string error;

		const auto& data = req->getJsonObject();

		if (!data) return callback(BadRequestException().response());

		validator schema({
			{"name", "type:string|required|minLength:3|alphabetOnly"},
			{"description", "type:string|required|minLength:3|alphabetOnly"},
			{"price", "type:number|required|numberOnly"},
			{"stock", "type:number|required|numberOnly"}
		});

		MasterProducts product(*data);

		if (!schema.validate(product.toJson(), error))
		{
			return callback(BadRequestException(error).response());
		}

		db().insert(product, [=](MasterProducts product) 
		{
			Json::Value json;

			json["message"] = "Create product success";
			json["success"] = true;

			auto response = HttpResponse::newHttpJsonResponse(json);

			return callback(response);
		}, [=](DrogonDbException const& e) 
		{
			LOG(WARNING) << e.base().what();

			Json::Value json;

			json["message"] = e.base().what();
			json["success"] = false;

			auto response = HttpResponse::newHttpJsonResponse(json);
			response->setStatusCode(HttpStatusCode::k500InternalServerError);

			return callback(response);
		});
	}
	void products::remove(HttpRequestPtr const& req, response_t&& callback, std::string&& id)
	{
		Json::Value resp;

		if (id.empty() || !util::is_numeric(id))
		{
			return callback(BadRequestException("Parameters requirement doesn't match").response());
		}

		db().deleteByPrimaryKey(stoll(id), [=](size_t record)
		{
			if (record != 0)
			{
				Json::Value json;
				json["message"] = fmt::format("Delete user on {} successfully", record);
				json["success"] = true;

				auto response = HttpResponse::newHttpJsonResponse(json);
				callback(response);
			}
			else
			{
				callback(NotFoundException("Unable to delete non-existing data").response());
			}

		}, [=](DrogonDbException const& e)
		{
			Json::Value json;
			json["message"] = fmt::format("Failed delete user, error caught on {}", e.base().what());
			json["success"] = false;

			auto response = HttpResponse::newHttpJsonResponse(json);
			response->setStatusCode(k500InternalServerError);

			callback(response);
		});
	}
}