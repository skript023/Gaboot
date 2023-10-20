#include "user.h"
#include "util/exception.hpp"

// Add definition of your processing function here
namespace gaboot
{
    void user::findAll(HttpRequestPtr const& req, response_t&& callback)
    {
        db().findAll([=](std::vector<Users> users)
        {
            Json::Value json;

            try
            {
                if (users.empty())
                {
                    throw  NotFoundException("No data retrieved");
                }

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
            catch (const GabootException& e)
            {
                json["message"] = fmt::format("Cannot retrieve user data, error caught on {}", e.what());
                json["success"] = false;
                json["data"] = Json::arrayValue;

                auto response = HttpResponse::newHttpJsonResponse(json);
                response->setStatusCode(e.get_code());

                callback(response);
            }
        }, [=](DrogonDbException const& e) 
        {
            Json::Value json;

            json["message"] = fmt::format("Cannot retrieve user data, error caught on {}", e.base().what());
            json["success"] = false;
            json["data"] = Json::arrayValue;

            auto response = HttpResponse::newHttpJsonResponse(json);
            response->setStatusCode(k500InternalServerError);

            callback(response);
        });
    }

    void user::findOne(HttpRequestPtr const& req, response_t&& callback, std::string&& id)
    {
        Json::Value json;

        try
        {
            if (id.empty() || !util::is_numeric(id))
            {
                throw BadRequestException("Unknown parameters");
            }

            db().findByPrimaryKey(stoll(id),  [=](Users user) 
            {
                Json::Value json_cb;
                json_cb["message"] = "Success retrieve user data";
                json_cb["success"] = true;
                json_cb["data"] = user.toJson();

                auto response = HttpResponse::newHttpJsonResponse(json_cb);

                callback(response);
            }, [=](DrogonDbException const& e) 
            {
                Json::Value json_cb;
                json_cb["message"] = fmt::format("Cannot retrieve user data, error caught on {}", e.base().what());
                json_cb["success"] = false;
                json_cb["data"] = {};

                auto response = HttpResponse::newHttpJsonResponse(json_cb);
                response->setStatusCode(k500InternalServerError);

                callback(response);
            });
        }
        catch (const GabootException& e)
        {
            json["message"] = fmt::format("Cannot retrieve user data, error caught on {}", e.what());
            json["success"] = false;
            json["data"] = {};

            auto response = HttpResponse::newHttpJsonResponse(json);
            response->setStatusCode(e.get_code());

            callback(response);
        }
    }

    void user::create(HttpRequestPtr const& req, response_t&& callback)
    {
        Json::Value json;
        std::string error;

        try
        {
            MultiPartParser fileUpload;

            if (fileUpload.parse(req) != 0 || fileUpload.getFiles().size() == 0)
            {
                throw BadRequestException("Requirement doesn't match");
            }

            auto& file = fileUpload.getFiles()[0];

            Json::Value data;
            Users user;
            auto parameters = fileUpload.getParameters();

            for (auto param = parameters.rbegin(); param != parameters.rend(); ++param)
            {
                data[param->first] = param->second;
            }

            std::string fullname = data["fullname"].asString();
            std::string username = data["username"].asString();
            std::string email = data["email"].asString();
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

            auto valid = Users::validateJsonForCreation(user.toJson(), error);

            if (!valid)
            {
                throw BadRequestException(error);
            }

            db().insert(user, [&json, file, callback](Users user)
            {
                json["message"] = "Create user success";
                json["success"] = true;

                auto response = HttpResponse::newHttpJsonResponse(json);
                file.save();

                callback(response);
            }, [&](DrogonDbException const& e)
            {
                json["message"] = e.base().what();
                json["success"] = false;

                auto response = HttpResponse::newHttpJsonResponse(json);
                response->setStatusCode(HttpStatusCode::k500InternalServerError);

                callback(response);
            });
        }
        catch (const GabootException& e)
        {
            json["message"] = e.what();
            json["success"] = false;

            auto response = HttpResponse::newHttpJsonResponse(json);
            response->setStatusCode(HttpStatusCode::k400BadRequest);

            callback(response);
        }
    }

    void user::update(HttpRequestPtr const& req, response_t&& callback, std::string&& id)
    {
        Json::Value resp;

        try
        {
            MultiPartParser multipart;

            if (id.empty() || !util::is_numeric(id))
            {
                throw BadRequestException("Invalid parameters");
            }

            if (multipart.parse(req) != 0)
            {
                throw BadRequestException("Requirement doesn't match");
            }

            auto &file = multipart.getFiles()[0];

            auto& parameters = multipart.getParameters();
            Json::Value data;

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

            auto args = Criteria(Users::Cols::_id, CompareOperator::EQ, stoll(id));

            std::map<Json::Value::Members, std::string> columnMapping = {
                {{Users::Cols::_fullName}, "fullname"},
                {{Users::Cols::_userName}, "username"},
                {{Users::Cols::_password}, "password"},
                {{Users::Cols::_imgPath}, "image_path"},
                {{Users::Cols::_imgThumbPath}, "thumb_path"},
                {{Users::Cols::_roleId}, "role_id"},
                {{Users::Cols::_isActive}, "is_active"},
            };
            
            // Loop through JSON members and update corresponding database columns
            for (const auto& [column, request] : columnMapping)
            {
                if (data.isMember(request))
                {
                    auto jsonValue = data[request];
                    if (!jsonValue.isNull())
                    {
                        auto record = db().updateFutureBy(column, args, jsonValue.asString());
                        
                        resp[request + "_updated"] = true;
                    }
                }
            }

            if (multipart.getFiles().size() > 0 && util::allowed_image(file.getFileExtension().data()))
            {
                data["image_path"] = file.getFileName();
                data["thumb_path"] = file.getFileName();
                LOG_INFO << "File saved.";
                file.save();
            }

            resp["message"] = data;
            resp["success"] = true;

            auto response = HttpResponse::newHttpJsonResponse(resp);
            callback(response);
        }
        catch(const GabootException& e)
        {
            callback(e.response());
        }
    }

    void user::remove(HttpRequestPtr const& req, response_t&& callback, std::string&& id)
    {
        Json::Value resp;

        try
        {
            if (id.empty() || !util::is_numeric(id))
            {
                throw BadRequestException("Invalid parameters");
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
                    callback(NotFoundException("Record not found").response());
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
        catch (const GabootException& e)
        {
            callback(e.response());
        }
    }
}