#include "customer.h"
#include "util/exception.hpp"

// Add definition of your processing function here
namespace gaboot
{
    void customer::findAll(HttpRequestPtr const& req, response_t&& callback)
    {
        db().findAll([=](std::vector<MasterCustomers> users)
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

    void customer::findOne(HttpRequestPtr const& req, response_t&& callback, std::string&& id)
    {
        Json::Value json;

        try
        {
            if (id.empty() || !util::is_numeric(id))
            {
                throw BadRequestException("Unknown parameters");
            }

            db().findByPrimaryKey(stoll(id),  [=](MasterCustomers user) 
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

    void customer::create(HttpRequestPtr const& req, response_t&& callback)
    {
        std::string error;

        MultiPartParser fileUpload;

        if (fileUpload.parse(req) != 0 || fileUpload.getFiles().size() == 0)
        {
            callback(BadRequestException("Requirement doesn't match").response());

            return;
        }

        auto& file = fileUpload.getFiles()[0];

        Json::Value data;
        MasterCustomers customer;
        auto parameters = fileUpload.getParameters();

        for (auto param = parameters.rbegin(); param != parameters.rend(); ++param)
        {
            data[param->first] = param->second;
        }

        std::string firstname = data["firstname"].asString();
        std::string lastname = data["lastname"].asString();
        std::string username = data["username"].asString();
        std::string email = data["email"].asString();
        std::string phone = data["phoneNumber"].asString();
        std::string address = data["addressDetail"].asString();
        std::string latitude = data["latitude"].asString();
        std::string longitude = data["longitude"].asString();
        std::string password = data["password"].asString();

        customer.setFirstname(firstname);
        customer.setLastname(lastname);
        customer.setUsername(username);
        customer.setEmail(email);
        customer.setPassword(bcrypt::generateHash(password));
        customer.setPhonenumber(phone);
        customer.setAddressdetail(address);
        customer.setLatitude(latitude);
        customer.setLongitude(longitude);
        customer.setCreatedat(trantor::Date::now());
        customer.setUpdatedat(trantor::Date::now());
        customer.setImagepath(file.getFileName());
        customer.setThumbnailpath(file.getFileName());
        customer.setIsactive(true);

        auto valid = MasterCustomers::validateJsonForCreation(customer.toJson(), error);

        if (!valid)
        {
            callback(BadRequestException(error).response());
        }

        db().insert(customer, [=](MasterCustomers customer)
        {
            Json::Value json;

            json["message"] = "Create customer success";
            json["success"] = true;

            auto response = HttpResponse::newHttpJsonResponse(json);
            file.save();

            callback(response);
        }, [=](DrogonDbException const& e)
        {
            Json::Value json;

            json["message"] = e.base().what();
            json["success"] = false;

            auto response = HttpResponse::newHttpJsonResponse(json);
            response->setStatusCode(HttpStatusCode::k500InternalServerError);

            callback(response);
        });
    }

    void customer::update(HttpRequestPtr const& req, response_t&& callback, std::string&& id)
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

            if (multipart.getFiles().size() > 0 && util::allowed_image(file.getFileExtension().data()))
            {
                data["imagePath"] = file.getFileName();
                data["thumbnailPath"] = file.getFileName();
            }

            auto args = Criteria(MasterCustomers::Cols::_id, CompareOperator::EQ, stoll(id));

            std::map<Json::Value::Members, std::string> columnMapping = {
                {{MasterCustomers::Cols::_firstname}, "firstname"},
                {{MasterCustomers::Cols::_lastname}, "lastname"},
                {{MasterCustomers::Cols::_username}, "username"},
                {{MasterCustomers::Cols::_email}, "email"},
                {{MasterCustomers::Cols::_phoneNumber}, "phoneNumber"},
                {{MasterCustomers::Cols::_addressDetail}, "addressDetail"},
                {{MasterCustomers::Cols::_latitude}, "latitude"},
                {{MasterCustomers::Cols::_longitude}, "longitude"},
                {{MasterCustomers::Cols::_password}, "password"},
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
                        
                        if (record.valid() || record.get() != 0)
                        {
                            resp[request + "_updated"] = true;
                            resp["message"] = "Success update customer data.";
                            resp["success"] = true;

                            auto response = HttpResponse::newHttpJsonResponse(resp);
                            callback(response);

                            return;
                        }
                        else
                        {
                            throw NotFoundException("Unable to update non-existing record.");
                        }
                    }
                }
            }

            if (multipart.getFiles().size() > 0 && util::allowed_image(file.getFileExtension().data()))
            {
                LOG_INFO << "File saved.";
                file.save();
            }

            resp["message"] = "Unhandled update costumer.";
            resp["success"] = false;

            auto response = HttpResponse::newHttpJsonResponse(resp);
            callback(response);
        }
        catch(const GabootException& e)
        {
            callback(e.response());
        }
    }

    void customer::remove(HttpRequestPtr const& req, response_t&& callback, std::string&& id)
    {
        Json::Value resp;

        if (id.empty() || !util::is_numeric(id))
        {
            callback(BadRequestException("Invalid parameters").response());

            return;
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
}