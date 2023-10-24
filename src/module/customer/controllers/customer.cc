#include "customer.h"
#include "util/upload.hpp"
#include "util/gaboot.hpp"
#include "util/exception.hpp"
#include "util/file_manager.hpp"

// Add definition of your processing function here
namespace gaboot
{
    void customer::findAll(HttpRequestPtr const& req, response_t&& callback)
    {
        auto& limitParam = req->getParameter("limit");
        auto& pageParam = req->getParameter("page");

        const size_t limit = limitParam.empty() && !util::is_numeric(limitParam) ? 10 : stoull(limitParam);
        const size_t page = pageParam.empty() && !util::is_numeric(pageParam) ? 0 : stoull(pageParam) - 1;

        db().orderBy(MasterCustomers::Cols::_firstname).limit(limit).offset(page * limit).findAll([=](std::vector<MasterCustomers> users)
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

                const size_t lastPage = users.size() / limit + (users.size() % limit) == 0 ? 0 : 1;

                json["message"] = "Success retreive users data";
                json["success"] = true;
                json["data"] = res;
                json["lastPage"] = lastPage;

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
                if (!user.getId()) return callback(NotFoundException("Unable retrieve customer detail").response());

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
            return callback(BadRequestException("Requirement doesn't match").response());
        }

        auto& file = fileUpload.getFiles()[0];

        Json::Value data;

        if (!util::multipart_tojson(fileUpload, data)) return callback(BadRequestException("Unknown error").response());

        validator schema({
            {"firstname", "type:string|required|minLength:3|alphabetOnly"},
            {"lastname", "type:string|required|minLength:3|alphabetOnly"},
            {"username", "type:string|required|minLength:5|alphanum"},
            {"email", "type:string|required|email"},
            {"password", "type:string|required|minLength:8"}
        });

        MasterCustomers customer(data);

        upload_file upload(file, customer.getValueOfUsername(), "customers");

        customer.setCreatedat(trantor::Date::now());
        customer.setUpdatedat(trantor::Date::now());
        customer.setImagepath(upload.get_image_path());
        customer.setThumbnailpath(upload.get_thumbnail_path());
        customer.setIsactive(true);
        
        if (!schema.validate(customer.toJson(), error))
        {
            return callback(BadRequestException(error).response());
        }

        db().insert(customer, [=](MasterCustomers customer)
        {
            Json::Value json;

            json["message"] = "Create customer success";
            json["success"] = true;

            auto response = HttpResponse::newHttpJsonResponse(json);
            
            upload.save();

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

    void customer::update(HttpRequestPtr const& req, response_t&& callback, std::string&& id)
    {
        Json::Value resp;
        Json::Value data;
        Json::Value updated(Json::objectValue);

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

            data["updatedAt"] = trantor::Date::now().toDbStringLocal();

            util::multipart_tojson(multipart, data);

            if (multipart.getFiles().size() > 0 && util::allowed_image(file.getFileExtension().data()))
            {
                data["imagePath"] = file.getFileName();
                data["thumbnailPath"] = file.getFileName();
            }

            auto args = Criteria(MasterCustomers::Cols::_id, CompareOperator::EQ, stoll(id));

            // Loop through JSON members and update corresponding database columns
            for (const auto& [column, request] : this->columnMapping)
            {
                if (data.isMember(request))
                {
                    auto& jsonValue = data[request];
                    if (!jsonValue.isNull())
                    {
                        auto record = db().updateFutureBy(column, args, jsonValue.asString());
                        
                        if (record.valid() && record.get())
                        {
                            updated[request + "_updated"] = "success";
                        }
                        else
                        {
                            throw NotFoundException("Unable to update non-existing record.");
                        }
                    }
                }
            }

            if (!updated.empty())
            {
                if (multipart.getFiles().size() > 0 && util::allowed_image(file.getFileExtension().data()))
                {
                    LOG_INFO << "File saved.";
                    file.save();
                }

                resp["data"] = updated;
                resp["message"] = "Success update customer data.";
                resp["success"] = true;

                auto response = HttpResponse::newHttpJsonResponse(resp);
                return callback(response);
            }
            else
            {
                throw BadRequestException("Unable to update costumer");
            }
        }
        catch (const GabootException& e)
        {
            LOG(WARNING) << e.what();

            return callback(e.response());
        }
        catch (const std::exception& e)
        {
            resp["message"] = fmt::format("Unable to update data, error caught on {}", e.what());
            resp["success"] = true;

            auto response = HttpResponse::newHttpJsonResponse(resp);

            LOG(WARNING) << e.what();

            return callback(response);
        }
    }

    void customer::remove(HttpRequestPtr const& req, response_t&& callback, std::string&& id)
    {
        Json::Value resp;

        if (id.empty() || !util::is_numeric(id))
        {
            return callback(BadRequestException("Invalid parameters").response());
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

    void customer::getImage(HttpRequestPtr const& req, response_t&& callback, std::string&& id)
    {
        if (id.empty() || !util::is_numeric(id))
        {
            return callback(BadRequestException("Invalid parameters").response());
        }

        db().findByPrimaryKey(stoll(id), [=](MasterCustomers customer) 
        {
            auto response = HttpResponse::newFileResponse(customer.getValueOfImagepath());

            callback(response);
        }, [=](DrogonDbException const& e) 
        {
            Json::Value json;
            json["message"] = fmt::format("Failed load user image, error caught on {}", e.base().what());
            json["success"] = false;

            auto response = HttpResponse::newHttpJsonResponse(json);
            response->setStatusCode(k500InternalServerError);

            callback(response);
        });
    }
}