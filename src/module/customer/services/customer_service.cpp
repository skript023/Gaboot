#include "customer_service.hpp"

#include "uploader/upload.hpp"
#include "util/gaboot.hpp"
#include "exception/exception.hpp"
#include "file_manager/file_manager.hpp"

#include "auth/cache/auth_manager.hpp"

namespace gaboot
{
	HttpResponsePtr customer_service::findAll(HttpRequestPtr const& req)
	{
        TRY_CLAUSE
        {
            auto& limitParam = req->getParameter("limit");
            auto& pageParam = req->getParameter("page");

            const size_t limit = limitParam.empty() && !util::is_numeric(limitParam) ? 10 : stoull(limitParam);
            const size_t page = pageParam.empty() && !util::is_numeric(pageParam) ? 0 : stoull(pageParam) - 1;

            const auto customers = db().orderBy(MasterCustomers::Cols::_firstname).limit(limit).offset(page * limit).findFutureAll().get();

            if (customers.empty())
            {
                m_response.m_message = "Customer empty";
                m_response.m_success = true;

                return HttpResponse::newHttpJsonResponse(m_response.to_json());
            }

            Json::Value data(Json::arrayValue);

            std::ranges::for_each(customers.begin(), customers.end(), [this](MasterCustomers const& customer) {
                m_response.m_data.append(customer.toJson());
            });

            const size_t lastPage = customers.size() / limit + (customers.size() % limit) == 0 ? 0 : 1;

            m_response.m_message = "Success retreive customers data";
            m_response.m_success = true;
            m_response.m_data = data;
            m_response.m_last_page = lastPage;

            return HttpResponse::newHttpJsonResponse(m_response.to_json());
        } EXCEPT_CLAUSE
	}
    HttpResponsePtr customer_service::create(HttpRequestPtr const& req)
    {
        TRY_CLAUSE
        {
            Json::Value data;
            MultiPartParser fileUpload;

            if (fileUpload.parse(req) != 0 || fileUpload.getFiles().size() == 0)
            {
                throw BadRequestException("Data is empty or file requirement doesn't match");
            }

            auto& file = fileUpload.getFiles()[0];

            if (!util::multipart_tojson(fileUpload, data)) throw BadRequestException("Unknown error");

            validator schema({
                {"firstname", "type:string|required|minLength:3|alphabetOnly"},
                {"lastname", "type:string|required|minLength:3|alphabetOnly"},
                {"username", "type:string|required|minLength:5|alphanum"},
                {"email", "type:string|required|email"},
                {"password", "type:string|required|minLength:8"}
            });

            MasterCustomers customer(data);

            upload_file upload(&file, customer.getValueOfUsername(), "customers");

            if (!schema.validate(customer.toJson(), m_error))
            {
                throw BadRequestException(m_error);
            }

            customer.setCreatedAt(trantor::Date::now());
            customer.setUpdatedAt(trantor::Date::now());
            customer.setImagePath(upload.get_image_path());
            customer.setThumbnailPath(upload.get_thumbnail_path());
            customer.setPassword(bcrypt::generateHash(customer.getValueOfPassword()));
            customer.setIsActive(true);

            db().insert(customer);

            LOG(INFO) << "Image saved at " << upload.get_image_path() << " thumbnail saved at " << upload.get_thumbnail_path();

            upload.save();

            m_response.m_message = "Create customer success";
            m_response.m_success = true;

            auto response = HttpResponse::newHttpJsonResponse(m_response.to_json());
            response->setStatusCode(k201Created);

            return response;
        } EXCEPT_CLAUSE
    }
    HttpResponsePtr customer_service::findOne(HttpRequestPtr const& req, std::string&& id)
    {
        TRY_CLAUSE
        {
            if (id.empty())
            {
                throw BadRequestException("Requirement doesn't match");
            }

            const auto user = db().findByPrimaryKey(id);

            if (!user.getId()) throw NotFoundException("Unable retrieve customer detail");

            m_response.m_message = "Success retrieve customers data";
            m_response.m_success = true;
            m_response.m_data = user.toJson();

            return HttpResponse::newHttpJsonResponse(m_response.to_json());
        } EXCEPT_CLAUSE
    }
    HttpResponsePtr customer_service::update(HttpRequestPtr const& req, std::string&& id) //bugged when upload file
    {
        TRY_CLAUSE
        {
            const auto& json = req->getJsonObject();

            if (!json) throw BadRequestException("Requested body can't be empty");

            if (id.empty())
            {
                throw BadRequestException("Parameters requirement doesn't match");
            }

            validator schema({
                {"firstname", "minLength:3|alphabetOnly"},
                {"lastname", "minLength:3|alphabetOnly"},
                {"username", "minLength:5|alphanum"},
                {"email", "email"},
                {"password", "minLength:8"}
                });

            if (!schema.validate(*json, m_error))
            {
                throw BadRequestException(m_error);
            }

            MasterCustomers customer = db().findByPrimaryKey(id);

            customer.updateByJson(*json);
            customer.setId(id);
            customer.setUpdatedAt(trantor::Date::now());

            if (!db().update(customer))
                throw BadRequestException("Unable to update non-existing record");

            m_response.m_message = "Success update customer data.";
            m_response.m_success = true;

            return HttpResponse::newHttpJsonResponse(m_response.to_json());
        } EXCEPT_CLAUSE
    }
    HttpResponsePtr customer_service::remove(HttpRequestPtr const& req, std::string&& id)
    {
        TRY_CLAUSE
        {
            if (id.empty())
            {
                LOG(WARNING) << "ID is empty or ID is not numeric";

                throw BadRequestException("Parameters requirement doesn't match");
            }

            if (!db().deleteByPrimaryKey(id))
            {
                throw BadRequestException("Unable delete non-existing data");
            }

            m_response.m_message = fmt::format("Delete customers on {} successfully", id);
            m_response.m_success = true;

            return HttpResponse::newHttpJsonResponse(m_response.to_json());
        } EXCEPT_CLAUSE
    }
    HttpResponsePtr customer_service::updateImage(HttpRequestPtr const& req, std::string&& id)
    {
        TRY_CLAUSE
        {
            MultiPartParser fileUpload;
            MasterCustomers customer = db().findByPrimaryKey(id);

            if (fileUpload.parse(req) != 0 || fileUpload.getFiles().size() == 0)
            {
                throw BadRequestException("Data is empty or file requirement doesn't match");
            }

            auto& file = fileUpload.getFiles()[0];

            upload_file upload(&file, customer.getValueOfUsername(), "customers");

            if (!util::allowed_image(file.getFileExtension().data()))
                throw BadRequestException("File type doesn't allowed");

            customer.setImagePath(upload.get_image_path());
            customer.setThumbnailPath(upload.get_thumbnail_path());
            customer.setUpdatedAt(trantor::Date::now());

            if (!db().update(customer))
            {
                throw InternalServerErrorException("Unable to update non-existing data");
            }

            upload.save();

            m_response.m_message = "Success update customer data.";
            m_response.m_success = true;

            return HttpResponse::newHttpJsonResponse(m_response.to_json());
        } EXCEPT_CLAUSE
    }
    HttpResponsePtr customer_service::getProfile(HttpRequestPtr const& req, std::string&& id)
    {
        TRY_CLAUSE
        {
            MasterCustomers customer;

            if (id.empty())
            {
                LOG(WARNING) << "ID is empty or ID is not numeric";

                throw BadRequestException();
            }

            if (!g_auth_manager->find(id, &customer))
            {
                throw NotFoundException("Unable to retreive customers profile");
            }

            auto customer_data = customer.toJson();
            customer_data.removeMember("password");
            customer_data.removeMember("updatedAt");
            customer_data.removeMember("isActive");
            customer_data.removeMember("token");

            m_response.m_data = customer_data;
            m_response.m_message = "Success retreive customers profile";
            m_response.m_success = true;

            return HttpResponse::newHttpJsonResponse(m_response.to_json());
        } EXCEPT_CLAUSE
    }
    HttpResponsePtr customer_service::getImage(HttpRequestPtr const& req, std::string&& id)
    {
        TRY_CLAUSE
        {
            MasterCustomers customer;

            if (id.empty())
            {
                LOG(WARNING) << "ID is empty or ID is not numeric";

                throw BadRequestException("Parameters requirement doesn't match");
            }

            if (g_auth_manager->find(id, &customer))
            {
                std::filesystem::path file(*customer.getImagePath());

                if (!std::filesystem::exists(file))
                    throw NotFoundException("Unable to retreive profile picture, please upload your profile picture");
                if (auto image = customer.getImagePath(); image && !image->empty())
                    return HttpResponse::newFileResponse(*customer.getImagePath());
            }

            throw NotFoundException("Unable to retreive customers image");
        } EXCEPT_CLAUSE
    }
    HttpResponsePtr customer_service::getThumbnail(HttpRequestPtr const& req, std::string&& id)
    {
        TRY_CLAUSE
        {
            MasterCustomers customer;

            if (id.empty())
            {
                LOG(WARNING) << "ID is empty or ID is not numeric";

                throw BadRequestException("Parameters requirement doesn't match");
            }

            if (g_auth_manager->find(id, &customer))
            {
                std::filesystem::path file(*customer.getThumbnailPath());

                if (!std::filesystem::exists(file))
                {
                    LOG(WARNING) << "File at " << file.lexically_normal() << " doesn't exist in server";

                    throw NotFoundException("Unable to retreive profile picture, please upload your profile picture");
                }
                if (auto image = customer.getImagePath(); image && !image->empty())
                    return HttpResponse::newFileResponse(*customer.getThumbnailPath());
            }

            throw NotFoundException("Unable to retreive customers image");
        } EXCEPT_CLAUSE
    }
}