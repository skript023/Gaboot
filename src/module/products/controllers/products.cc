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
				callback(NotFoundException("No data retrieved").response());
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

			json["message"] = fmt::format("Cannot retrieve user data, error caught on {}", e.base().what());
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
			return callback(BadRequestException("Unknown parameters").response());
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
			json_cb["message"] = fmt::format("Cannot retrieve user data, error caught on {}", e.base().what());
			json_cb["success"] = false;

			auto response = HttpResponse::newHttpJsonResponse(json_cb);
			response->setStatusCode(k500InternalServerError);

			return callback(response);
		});
	}
	void products::update(HttpRequestPtr const&, response_t&&, std::string&& id)
	{

	}
	void products::create(HttpRequestPtr const& req, response_t&& callback)
	{
		std::string error;

		MultiPartParser fileUpload;

		if (fileUpload.parse(req) != 0 || fileUpload.getFiles().size() == 0)
		{
			return callback(BadRequestException("Requirement doesn't match").response());
		}

		auto& file = fileUpload.getFiles()[0];

		Json::Value data;
		MasterProducts customer;

		util::multipart_tojson(fileUpload, data);

		MasterProducts product(data);

		validator schema({
			{"name", "type:string|required|minLength:3|alphabetOnly"},
			{"description", "type:string|required|minLength:3|alphabetOnly"}
		});

		auto valid = schema.validate(data, error);
		if (!valid)
		{
			return callback(BadRequestException(error).response());
		}
	}
	void products::remove(HttpRequestPtr const& req, response_t&& callback, std::string&& id)
	{

	}
}