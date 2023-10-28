#include "customer_service.hpp"

#include "util/upload.hpp"
#include "util/gaboot.hpp"
#include "util/exception.hpp"
#include "util/file_manager.hpp"
#include "module/customer/services/customer_manager.hpp"

namespace gaboot
{
	HttpResponsePtr customer_service::findAll(HttpRequestPtr const& req)
	{
        Json::Value json;

        auto& limitParam = req->getParameter("limit");
        auto& pageParam = req->getParameter("page");

        const size_t limit = limitParam.empty() && !util::is_numeric(limitParam) ? 10 : stoull(limitParam);
        const size_t page = pageParam.empty() && !util::is_numeric(pageParam) ? 0 : stoull(pageParam) - 1;

        try
        {
            auto users = db().orderBy(MasterCustomers::Cols::_firstname).limit(limit).offset(page * limit).findFutureAll().get();

            if (users.empty())
            {
                return NotFoundException("No data retrieved").response();
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

            return HttpResponse::newHttpJsonResponse(json);
        }
        catch (const DrogonDbException& e)
        {
            json["message"] = fmt::format("Cannot retrieve user data, error caught on {}", e.base().what());
            json["success"] = false;
            json["data"] = Json::arrayValue;

            auto response = HttpResponse::newHttpJsonResponse(json);
            response->setStatusCode(k500InternalServerError);

            return response;
        }
	}
    HttpResponsePtr customer_service::create(HttpRequestPtr const& req)
    {
        std::string error;
        Json::Value json;
        Json::Value data;

        MultiPartParser fileUpload;

        try
        {
            if (fileUpload.parse(req) != 0 || fileUpload.getFiles().size() == 0)
            {
                return BadRequestException("Requirement doesn't match").response();
            }

            auto& file = fileUpload.getFiles()[0];

            if (!util::multipart_tojson(fileUpload, data)) return BadRequestException("Unknown error").response();

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
                return BadRequestException(error).response();
            }

            db().insert(customer);

            json["message"] = "Create customer success";
            json["success"] = true;

            auto response = HttpResponse::newHttpJsonResponse(json);

            upload.save();

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
    HttpResponsePtr customer_service::findOne(HttpRequestPtr const& req, std::string&& id)
    {
        Json::Value json_cb;

        if (id.empty() || !util::is_numeric(id))
        {
            return BadRequestException("Requirement doesn't match").response();
        }

        try
        {
            const auto user = db().findByPrimaryKey(stoll(id));

            if (!user.getId()) return NotFoundException("Unable retrieve customer detail").response();

            json_cb["message"] = "Success retrieve user data";
            json_cb["success"] = true;
            json_cb["data"] = user.toJson();

            auto response = HttpResponse::newHttpJsonResponse(json_cb);

            return response;
        }
        catch (const DrogonDbException& e)
        {
            json_cb["message"] = fmt::format("Cannot retrieve user data, error caught on {}", e.base().what());
            json_cb["success"] = false;

            auto response = HttpResponse::newHttpJsonResponse(json_cb);
            response->setStatusCode(k500InternalServerError);

            return response;
        }
    }
    HttpResponsePtr customer_service::update(HttpRequestPtr const& req, std::string&& id)
    {
        Json::Value resp;
        Json::Value data;
        Json::Value updated(Json::objectValue);

        try
        {
            MultiPartParser multipart;

            if (id.empty() || !util::is_numeric(id))
            {
                return BadRequestException("Parameters requirement doesn't match").response();
            }

            if (multipart.parse(req) != 0)
            {
                return BadRequestException("Requirement doesn't match").response();
            }

            auto& file = multipart.getFiles()[0];

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
                            return NotFoundException("Unable to update non-existing record.").response();
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
                return response;
            }
            else
            {
                return BadRequestException("Unable to update costumer").response();
            }
        }
        catch (const DrogonDbException& e)
        {
            resp["message"] = fmt::format("Unable to update data, error caught on {}", e.base().what());
            resp["success"] = true;

            auto response = HttpResponse::newHttpJsonResponse(resp);
            response->setStatusCode(k500InternalServerError);

            LOG(WARNING) << e.base().what();

            return response;
        }
    }
    HttpResponsePtr customer_service::remove(HttpRequestPtr const& req, std::string&& id)
    {
        Json::Value json;

        if (id.empty() || !util::is_numeric(id))
        {
            return BadRequestException("Parameters requirement doesn't match").response();
        }

        try
        {
            const auto record = db().deleteByPrimaryKey(stoll(id));
            if (record != 0)
            {
                json["message"] = fmt::format("Delete user on {} successfully", record);
                json["success"] = true;

                return HttpResponse::newHttpJsonResponse(json);
            }

            return NotFoundException("Record not found").response();
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
    HttpResponsePtr customer_service::getProfile(HttpRequestPtr const& req, std::string&& id)
    {
        Json::Value resp;
        MasterCustomers customer;

        if (id.empty() || !util::is_numeric(id))
        {
            return BadRequestException().response();
        }

        if (g_customer_manager->find(stoll(id), &customer))
        {
            auto customer_data = customer.toJson();
            customer_data.removeMember("password");
            customer_data.removeMember("updatedAt");
            customer_data.removeMember("isActive");
            customer_data.removeMember("token");

            resp["data"] = customer_data;
            resp["message"] = "Success retreive user profile";
            resp["success"] = true;

            auto response = HttpResponse::newHttpJsonResponse(resp);

            return response;
        }

        resp["message"] = "Unable to retreive user profile";
        resp["success"] = false;

        auto response = HttpResponse::newHttpJsonResponse(resp);
        response->setStatusCode(k404NotFound);

        return response;
    }
    HttpResponsePtr customer_service::getImage(HttpRequestPtr const& req, std::string&& id)
    {
        MasterCustomers customer;
        Json::Value json;

        if (id.empty() || !util::is_numeric(id))
        {
            return BadRequestException("Parameters requirement doesn't match").response();
        }

        if (g_customer_manager->find(stoll(id), &customer))
        {
            auto customer_data = customer.toJson();
            customer_data.removeMember("password");
            customer_data.removeMember("updatedAt");
            customer_data.removeMember("isActive");
            customer_data.removeMember("token");

            return HttpResponse::newFileResponse(customer.getValueOfImagepath());
        }

        json["message"] = "Unable to retreive user image";
        json["success"] = false;

        auto response = HttpResponse::newHttpJsonResponse(json);
        response->setStatusCode(k404NotFound);

        return response;
    }
}