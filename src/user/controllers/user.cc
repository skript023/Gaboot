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

        MultiPartParser fileUpload;

        if (fileUpload.parse(req) != 0 || fileUpload.getFiles().size() == 0) 
        {
            // The framework handles an exception by logging it, and
            // by responding to the client with an HTTP 500 status code.
            throw std::runtime_error("Something went wrong");
	    }

        auto &file = fileUpload.getFiles()[0];
	    file.save();
        
        Json::Value data;
        Users user;
        auto parameters = fileUpload.getParameters();

        for (auto param = parameters.rbegin(); param != parameters.rend(); ++param)
        {
            data[param->first] = param->second;
        }
        
        std::string fullname = data["fullname"].asString();
        std::string username = data["username"].asString();
        std::string email    = data["email"].asString();
        std::string password = data["password"].asString();
        
        user.setFullname(fullname);
        user.setUsername(username);
        user.setPassword(bcrypt::generateHash(password));
        user.setIsactive(true);
        user.setRoleid(1);
        user.setCreatedat(trantor::Date::now());
        user.setUpdatedat(trantor::Date::now());
        user.setImgpath(file.getFileName());
        user.setImgthumbpath(file.getFileName());

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