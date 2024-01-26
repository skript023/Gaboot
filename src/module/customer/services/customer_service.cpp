#include "customer_service.hpp"

#include "uploader/upload.hpp"
#include "util/gaboot.hpp"
#include "exception/exception.hpp"
#include "file_manager/file_manager.hpp"

#include "customer/services/customer_manager.hpp"

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
                return BadRequestException("Data is empty or file requirement doesn't match").response();
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

            upload_file upload(&file, customer.getValueOfUsername(), "customers");

            if (!schema.validate(customer.toJson(), m_error))
            {
                return BadRequestException(m_error).response();
            }

            customer.setCreatedat(trantor::Date::now());
            customer.setUpdatedat(trantor::Date::now());
            customer.setImgpath(upload.get_image_path());
            customer.setImgthumbpath(upload.get_thumbnail_path());
            customer.setPassword(bcrypt::generateHash(customer.getValueOfPassword()));
            customer.setIsactive(true);

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
            if (id.empty() || !util::is_numeric(id))
            {
                return BadRequestException("Requirement doesn't match").response();
            }

            const auto user = db().findByPrimaryKey(stoll(id));

            if (!user.getId()) return NotFoundException("Unable retrieve customer detail").response();

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
            MultiPartParser multipart;
            MasterCustomers customer;

            if (id.empty() || !util::is_numeric(id))
            {
                return BadRequestException("Parameters requirement doesn't match").response();
            }

            if (multipart.parse(req) != 0)
            {
                return BadRequestException("Requirement doesn't match").response();
            }

            auto& file = multipart.getFiles()[0];

            m_data["updatedAt"] = trantor::Date::now().toDbStringLocal();

            util::multipart_tojson(multipart, m_data);

            g_customer_manager->find(stoll(id), &customer);

            upload_file upload(&file, customer.getValueOfUsername(), "customers");

            if (multipart.getFiles().size() > 0 && util::allowed_image(file.getFileExtension().data()))
            {
                m_data["imagePath"] = upload.get_image_path();
                m_data["thumbnailPath"] = upload.get_thumbnail_path();
            }

            customer.updateByJson(m_data);

            validator schema({
                {"firstname", "type:string|required|minLength:3|alphabetOnly"},
                {"lastname", "type:string|required|minLength:3|alphabetOnly"},
                {"username", "type:string|required|minLength:5|alphanum"},
                {"email", "type:string|required|email"},
                {"password", "type:string|required|minLength:8"}
            });

            if (!schema.validate(customer.toJson(), m_error))
            {
                return BadRequestException(m_error).response();
            }

            customer.setPassword(bcrypt::generateHash(customer.getValueOfPassword()));

            if (db().updateFuture(customer).get())
            {
                if (!g_customer_manager->update(stoll(id), customer))
                    LOG(WARNING) << "Authentication cache update failed.";

                if (multipart.getFiles().size() > 0 && util::allowed_image(file.getFileExtension().data()))
                {
                    upload.save();

                    LOG(INFO) << "Image saved at " << upload.get_image_path() << " thumbnail saved at " << upload.get_thumbnail_path();
                }
            }

            m_response.m_message = "Success update customer data.";
            m_response.m_success = true;

            return HttpResponse::newHttpJsonResponse(m_response.to_json());
        } EXCEPT_CLAUSE
    }
    HttpResponsePtr customer_service::remove(HttpRequestPtr const& req, std::string&& id)
    {
        TRY_CLAUSE
        {
            if (id.empty() || !util::is_numeric(id))
            {
                LOG(WARNING) << "ID is empty or ID is not numeric";

                return BadRequestException("Parameters requirement doesn't match").response();
            }

            const auto record = db().deleteByPrimaryKey(stoll(id));
            if (record != 0)
            {
                m_response.m_message = fmt::format("Delete customers on {} successfully", record);
                m_response.m_success = true;

                return HttpResponse::newHttpJsonResponse(m_response.to_json());
            }

            return NotFoundException("Record not found").response();
        } EXCEPT_CLAUSE
    }
    HttpResponsePtr customer_service::getProfile(HttpRequestPtr const& req, std::string&& id)
    {
        TRY_CLAUSE
        {
            MasterCustomers customer;

            if (id.empty() || !util::is_numeric(id))
            {
                LOG(WARNING) << "ID is empty or ID is not numeric";

                throw BadRequestException();
            }

            if (!g_customer_manager->find(stoll(id), &customer))
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

            if (id.empty() || !util::is_numeric(id))
            {
                LOG(WARNING) << "ID is empty or ID is not numeric";

                throw BadRequestException("Parameters requirement doesn't match");
            }

            if (g_customer_manager->find(stoll(id), &customer))
            {
                std::filesystem::path file(*customer.getImgpath());

                if (!std::filesystem::exists(file))
                    throw NotFoundException("Unable to retreive profile picture, please upload your profile picture");
                if (auto image = customer.getImgpath(); image && !image->empty())
                    return HttpResponse::newFileResponse(*customer.getImgpath());
            }

            throw NotFoundException("Unable to retreive customers image");
        } EXCEPT_CLAUSE
    }
    HttpResponsePtr customer_service::getThumbnail(HttpRequestPtr const& req, std::string&& id)
    {
        TRY_CLAUSE
        {
            MasterCustomers customer;

            if (id.empty() || !util::is_numeric(id))
            {
                LOG(WARNING) << "ID is empty or ID is not numeric";

                throw BadRequestException("Parameters requirement doesn't match");
            }

            if (g_customer_manager->find(stoll(id), &customer))
            {
                std::filesystem::path file(*customer.getImgthumbpath());

                if (!std::filesystem::exists(file))
                {
                    LOG(WARNING) << "File at " << file.lexically_normal() << " doesn't exist in server";

                    throw NotFoundException("Unable to retreive profile picture, please upload your profile picture");
                }
                if (auto image = customer.getImgpath(); image && !image->empty())
                    return HttpResponse::newFileResponse(*customer.getImgthumbpath());
            }

            throw NotFoundException("Unable to retreive customers image");
        } EXCEPT_CLAUSE
    }
}