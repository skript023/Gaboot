#include "user.h"

// Add definition of your processing function here
namespace gaboot
{
    void user::findAll(HttpRequestPtr const& req, response_t&& callback)
    {
        Json::Value json;

        try
        {
            auto users = db().findAll();
            
            Json::Value res(Json::arrayValue);

            for (const auto& user : users) 
            {
                res.append(user.toJson());
            }

            json["message"] = "Success retreive users data";
            json["success"] = true;
            json["data"] = res;
            auto response = HttpResponse::newHttpJsonResponse(json);
            
            callback(response);
        }
        catch(const std::exception& e)
        {
            json["message"] = std::format("Cannot retrieve user data, error caught on {}", e.what());
            json["success"] = false;
            json["data"] = Json::arrayValue;

            const auto& func = [json](HttpStatusCode code) -> HttpResponsePtr
            {
                auto response = HttpResponse::newHttpJsonResponse(json);
                response->setStatusCode(code);

                return response;
            };

            auto& handler = app().setCustomErrorHandler(func).getCustomErrorHandler();

            callback(handler(HttpStatusCode::k404NotFound));
        }
        
    }

    void user::findOne(HttpRequestPtr const& req, response_t&& callback, long long id)
    {
        Json::Value json;

        try
        {
            auto user = db().findByPrimaryKey(id);
            json["message"] = "Success retrieve user data";
            json["success"] = true;
            json["data"] = user.toJson();

            auto response = HttpResponse::newHttpJsonResponse(json);
            
            callback(response);
        }
        catch(const std::exception& e)
        {
            json["message"] = std::format("Cannot retrieve user data, error caught on {}", e.what());
            json["success"] = false;
            json["data"] = {};

            const auto& func = [json](HttpStatusCode code) -> HttpResponsePtr 
            {
                auto response = HttpResponse::newHttpJsonResponse(json);
                response->setStatusCode(code);

                return response;
            };

            auto& handler = app().setCustomErrorHandler(func).getCustomErrorHandler();

            callback(handler(HttpStatusCode::k404NotFound));
        }
    }

    void user::create(HttpRequestPtr const& req, response_t&& callback)
    {
        Json::Value json;
        std::string error;

        const auto& body = *req->getJsonObject().get();
            
        Users user;
        std::string fullname = body["fullname"].asString();
        std::string username = body["username"].asString();
        std::string password = body["password"].asString();

        user.setFullname(fullname);
        user.setUsername(username);
        user.setPassword(bcrypt::generateHash(password));
        user.setIsactive(true);
        user.setRoleid(1);
        user.setCreatedat(trantor::Date::now());
        user.setUpdatedat(trantor::Date::now());

        auto validate = Users::validateJsonForCreation(user.toJson(), error);
        
        if (validate)
        {
            db().insert(user);
            json["message"] = "Create user success";
            json["success"] = true;

            auto response = HttpResponse::newHttpJsonResponse(json);

            callback(response);
        }
        else
        {
            json["message"] = error;
            json["success"] = validate;

            auto response = HttpResponse::newHttpJsonResponse(json);
            response->setStatusCode(HttpStatusCode::k400BadRequest);
            callback(response);
        }        
    }

    void user::update(HttpRequestPtr const& req, response_t&& callback, int64_t id)
    {
        
    }
}