#include "user.h"

// Add definition of your processing function here
namespace gaboot
{
    void user::findAll(HttpRequestPtr const& req, response_t&& callback)
    {
        Json::Value json;

        try
        {
            auto users = db().findFutureAll();
            
            Json::Value res(Json::arrayValue);

            for (const auto& user : users.get()) 
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
        Json::Value resp;
        Json::Value data;
        
        MultiPartParser fileUpload;

        if (fileUpload.parse(req) != 0 || fileUpload.getFiles().size() == 0) 
        {
            // The framework handles an exception by logging it, and
            // by responding to the client with an HTTP 500 status code.
            throw std::runtime_error("Something went wrong");
	    }

        auto &file = fileUpload.getFiles()[0];
	    file.save();

        auto parameters = fileUpload.getParameters();

        for (auto param = parameters.rbegin(); param != parameters.rend(); ++param)
        {
            if (param->first == "password")
            {
                data[param->first] = bcrypt::generateHash(param->second);
            }
            else
            {
                data[param->first] = param->second;
            }
        }

        auto args = Criteria(Users::Cols::_id, CompareOperator::EQ, id);

        std::map<Json::Value::Members, std::string> columnMapping = {
            {{Users::Cols::_fullName}, "fullname"},
            {{Users::Cols::_userName}, "username"},
            {{Users::Cols::_password}, "password"},
            {{Users::Cols::_imgPath}, "image_path"},
            {{Users::Cols::_imgThumbPath}, "thumb_path"},
            {{Users::Cols::_roleId}, "role_id"},
            {{Users::Cols::_isActive}, "is_active"},
        };

        try
        {
            if (id == NULL)
            {
                throw std::runtime_error("Invalid parameters");
            }
            // Loop through JSON members and update corresponding database columns
            for (const auto& [column, request] : columnMapping)
            {
                if (data.isMember(request))
                {
                    auto jsonValue = data[request];
                    if (!jsonValue.isNull())
                    {
                        db().updateBy(column, args, jsonValue.asString());
                        resp[request + "_updated"] = true;
                    }
                }
            }

            resp["message"] = "Update activity successful";
            resp["success"] = true;

            auto response = HttpResponse::newHttpJsonResponse(resp);
            callback(response);
        }
        catch(const std::exception& e)
        {
            resp["message"] = std::format("Update activity failed, error caught on {}", e.what());
            resp["success"] = false;
            
            auto response = HttpResponse::newHttpJsonResponse(resp);
            response->setStatusCode(HttpStatusCode::k500InternalServerError);

            callback(response);
        }
    }

    void user::remove(HttpRequestPtr const& req, response_t&& callback, int64_t id)
    {
        Json::Value resp;
        try
        {
            auto record = db().deleteFutureByPrimaryKey(id);

            resp["message"] = "Delete user successfully";
            resp["success"] = true;

            auto response = HttpResponse::newHttpJsonResponse(resp);
            callback(response);
        }
        catch(const std::exception& e)
        {
            resp["message"] = std::format("Failed delete user, error caught on {}", e.what());
            resp["success"] = false;

            auto& handler = app().setCustomErrorHandler([=](HttpStatusCode code) -> HttpResponsePtr
            {
                auto response = HttpResponse::newHttpJsonResponse(resp);
                response->setStatusCode(code);

                return response;
            }).getCustomErrorHandler();

            auto ex_code = handler(k400BadRequest);
            callback(ex_code);
        }
        
    }
}