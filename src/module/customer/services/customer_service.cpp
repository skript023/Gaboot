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

            m_response.m_message = "Success retreive users data";
            m_response.m_success = true;
            m_response.m_data = res;
            m_response.m_last_page = lastPage;

            return HttpResponse::newHttpJsonResponse(m_response.to_json());
        }
        catch (const DrogonDbException& e)
        {
            m_response.m_message = fmt::format("Cannot retrieve user data, error caught on {}", e.base().what());
            m_response.m_success = false;
            m_response.m_data = Json::arrayValue;

            auto response = HttpResponse::newHttpJsonResponse(m_response.to_json());
            response->setStatusCode(k500InternalServerError);

            return response;
        }
	}
    HttpResponsePtr customer_service::create(HttpRequestPtr const& req)
    {
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

            if (!schema.validate(customer.toJson(), m_error))
            {
                return BadRequestException(m_error).response();
            }

            db().insert(customer);

            m_response.m_message = "Create customer success";
            m_response.m_success = true;

            auto response = HttpResponse::newHttpJsonResponse(m_response.to_json());

            upload.save();

            return response;
        }
        catch (const DrogonDbException& e)
        {
            LOG(WARNING) << e.base().what();

            m_response.m_message = e.base().what();
            m_response.m_success = false;

            auto response = HttpResponse::newHttpJsonResponse(m_response.to_json());
            response->setStatusCode(HttpStatusCode::k500InternalServerError);

            return response;
        }
    }
    HttpResponsePtr customer_service::findOne(HttpRequestPtr const& req, std::string&& id)
    {
        try
        {
            if (id.empty() || !util::is_numeric(id))
            {
                return BadRequestException("Requirement doesn't match").response();
            }

            const auto user = db().findByPrimaryKey(stoll(id));

            if (!user.getId()) return NotFoundException("Unable retrieve customer detail").response();

            m_response.m_message = "Success retrieve user data";
            m_response.m_success = true;
            m_response.m_data = user.toJson();

            return HttpResponse::newHttpJsonResponse(m_response.to_json());
        }
        catch (const DrogonDbException& e)
        {
            m_response.m_message = fmt::format("Cannot retrieve user data, error caught on {}", e.base().what());
            m_response.m_success = false;

            auto response = HttpResponse::newHttpJsonResponse(m_response.to_json());
            response->setStatusCode(k500InternalServerError);

            return response;
        }
    }
    HttpResponsePtr customer_service::update(HttpRequestPtr const& req, std::string&& id)
    {
        Json::Value data;

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
                            m_response.m_data = fmt::format("{} success", request);
                        }
                        else
                        {
                            return NotFoundException("Unable to update non-existing record.").response();
                        }
                    }
                }
            }

            if (!m_response.m_data.empty())
            {
                if (multipart.getFiles().size() > 0 && util::allowed_image(file.getFileExtension().data()))
                {
                    LOG_INFO << "File saved.";
                    file.save();
                }

                m_response.m_message = "Success update customer data.";
                m_response.m_success = true;

                auto response = HttpResponse::newHttpJsonResponse(m_response.to_json());
                return response;
            }
            else
            {
                return BadRequestException("Unable to update costumer").response();
            }
        }
        catch (const DrogonDbException& e)
        {
            m_response.m_message = fmt::format("Unable to update data, error caught on {}", e.base().what());
            m_response.m_success = true;

            auto response = HttpResponse::newHttpJsonResponse(m_response.to_json());
            response->setStatusCode(k500InternalServerError);

            LOG(WARNING) << e.base().what();

            return response;
        }
    }
    HttpResponsePtr customer_service::remove(HttpRequestPtr const& req, std::string&& id)
    {
        if (id.empty() || !util::is_numeric(id))
        {
            return BadRequestException("Parameters requirement doesn't match").response();
        }

        try
        {
            const auto record = db().deleteByPrimaryKey(stoll(id));
            if (record != 0)
            {
                m_response.m_message = fmt::format("Delete user on {} successfully", record);
                m_response.m_success = true;

                return HttpResponse::newHttpJsonResponse(m_response.to_json());
            }

            return NotFoundException("Record not found").response();
        }
        catch (const DrogonDbException& e)
        {
            m_response.m_message = fmt::format("Failed delete user, error caught on {}", e.base().what());
            m_response.m_success = false;

            auto response = HttpResponse::newHttpJsonResponse(m_response.to_json());
            response->setStatusCode(k500InternalServerError);

            return response;
        }
    }
    HttpResponsePtr customer_service::getProfile(HttpRequestPtr const& req, std::string&& id)
    {
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

            m_response.m_data = customer_data;
            m_response.m_message = "Success retreive user profile";
            m_response.m_success = true;

            auto response = HttpResponse::newHttpJsonResponse(m_response.to_json());

            return response;
        }

        m_response.m_message = "Unable to retreive user profile";
        m_response.m_success = false;

        auto response = HttpResponse::newHttpJsonResponse(m_response.to_json());
        response->setStatusCode(k404NotFound);

        return response;
    }
    HttpResponsePtr customer_service::getImage(HttpRequestPtr const& req, std::string&& id)
    {
        MasterCustomers customer;

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

        m_response.m_message = "Unable to retreive user image";
        m_response.m_success = false;

        auto response = HttpResponse::newHttpJsonResponse(m_response.to_json());
        response->setStatusCode(k404NotFound);

        return response;
    }
}