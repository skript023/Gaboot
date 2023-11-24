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
            const auto customers = db().orderBy(MasterCustomers::Cols::_firstname).limit(limit).offset(page * limit).findFutureAll().get();

            if (customers.empty())
            {
                m_response.m_message = "No data retrieved";
                m_response.m_success = false;

                return HttpResponse::newHttpJsonResponse(m_response.to_json());
            }

            Json::Value data(Json::arrayValue);

            std::ranges::for_each(customers.begin(), customers.end(), [&data](MasterCustomers const& customer) {
                data.append(customer.toJson());
            });

            const size_t lastPage = customers.size() / limit + (customers.size() % limit) == 0 ? 0 : 1;

            m_response.m_message = "Success retreive customers data";
            m_response.m_success = true;
            m_response.m_data = data;
            m_response.m_last_page = lastPage;

            return HttpResponse::newHttpJsonResponse(m_response.to_json());
        }
        catch (const DrogonDbException& e)
        {
            LOG(WARNING) << fmt::format("Cannot retrieve customers data, error caught on {}", e.base().what());

            m_response.m_message = fmt::format("Cannot retrieve customers data, error caught on {}", e.base().what());
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

        try
        {
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

            customer.setCreatedat(trantor::Date::now());
            customer.setUpdatedat(trantor::Date::now());
            customer.setImgpath(upload.get_image_path());
            customer.setImgthumbpath(upload.get_thumbnail_path());
            customer.setIsactive(true);

            if (!schema.validate(customer.toJson(), m_error))
            {
                return BadRequestException(m_error).response();
            }

            db().insertFuture(customer);

            LOG(INFO) << "Image saved at " << upload.get_image_path() << " thumbnail saved at " << upload.get_thumbnail_path();

            upload.save();

            m_response.m_message = "Create customer success";
            m_response.m_success = true;

            auto response = HttpResponse::newHttpJsonResponse(m_response.to_json());

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

            m_response.m_message = "Success retrieve customers data";
            m_response.m_success = true;
            m_response.m_data = user.toJson();

            return HttpResponse::newHttpJsonResponse(m_response.to_json());
        }
        catch (const DrogonDbException& e)
        {
            LOG(WARNING) << fmt::format("Cannot retrieve customers data, error caught on {}", e.base().what());

            m_response.m_message = fmt::format("Cannot retrieve customers data, error caught on {}", e.base().what());
            m_response.m_success = false;

            auto response = HttpResponse::newHttpJsonResponse(m_response.to_json());
            response->setStatusCode(k500InternalServerError);

            return response;
        }
    }
    HttpResponsePtr customer_service::update(HttpRequestPtr const& req, std::string&& id)
    {
        MasterCustomers customer;
        MultiPartParser multipart;

        try
        {
            if (id.empty() || !util::is_numeric(id))
            {
                return BadRequestException("Parameters empty or doesn't match").response();
            }

            if (multipart.parse(req) != 0)
            {
                return BadRequestException("Data is empty or file requirement doesn't match").response();
            }

            auto& file = multipart.getFiles()[0];

            if (!g_customer_manager->find(stoll(id), &customer))
                return UnauthorizedException("You're not logged in, please login!").response();

            upload_file upload(&file, customer.getValueOfUsername(), "customers");

            util::multipart_tojson(multipart, m_data);

            if (!util::allowed_image(upload.get_image_filename()))
                return BadRequestException("File type is not allowerd").response();

            m_data["imgPath"] = upload.get_image_path();
            m_data["imgThumbPath"] = upload.get_thumbnail_path();
            m_data["updatedAt"] = trantor::Date::now().toDbStringLocal();

            customer.updateByJson(m_data);

            if (auto record = db().updateFuture(customer).get(); !record)
                return BadRequestException("Unable to update non-existing record").response();

            LOG(INFO) << "Image saved at " << upload.get_image_path() << " thumbnail saved at " << upload.get_thumbnail_path();

            if (multipart.getFiles().size() != 0)
                upload.save();

            m_response.m_message = "Success update customer data.";
            m_response.m_success = true;

            g_customer_manager->remove(stoll(id));

            auto response = HttpResponse::newHttpJsonResponse(m_response.to_json());
            return response;
        }
        catch (const DrogonDbException& e)
        {
            LOG(WARNING) << fmt::format("Unable to update data, error caught on {}", e.base().what());

            m_response.m_message = fmt::format("Unable to update data, error caught on {}", e.base().what());
            m_response.m_success = true;

            auto response = HttpResponse::newHttpJsonResponse(m_response.to_json());
            response->setStatusCode(k500InternalServerError);

            return response;
        }
    }
    HttpResponsePtr customer_service::remove(HttpRequestPtr const& req, std::string&& id)
    {
        if (id.empty() || !util::is_numeric(id))
        {
            LOG(WARNING) << "ID is empty or ID is not numeric";

            return BadRequestException("Parameters requirement doesn't match").response();
        }

        try
        {
            const auto record = db().deleteByPrimaryKey(stoll(id));
            if (record != 0)
            {
                m_response.m_message = fmt::format("Delete customers on {} successfully", record);
                m_response.m_success = true;

                return HttpResponse::newHttpJsonResponse(m_response.to_json());
            }

            return NotFoundException("Record not found").response();
        }
        catch (const DrogonDbException& e)
        {
            LOG(WARNING) << fmt::format("Failed delete customers, error caught on {}", e.base().what());

            m_response.m_message = fmt::format("Failed delete customers, error caught on {}", e.base().what());
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
            LOG(WARNING) << "ID is empty or ID is not numeric";

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
            m_response.m_message = "Success retreive customers profile";
            m_response.m_success = true;

            auto response = HttpResponse::newHttpJsonResponse(m_response.to_json());

            return response;
        }

        m_response.m_message = "Unable to retreive customers profile";
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
            LOG(WARNING) << "ID is empty or ID is not numeric";

            return BadRequestException("Parameters requirement doesn't match").response();
        }

        if (g_customer_manager->find(stoll(id), &customer))
        {
            std::filesystem::path file(*customer.getImgpath());

            if (!std::filesystem::exists(file))
            {
                LOG(WARNING) << "File at " << file.lexically_normal() << " doesn't exist in server";

                m_response.m_message = "Unable to retreive profile picture, please upload your profile picture";
                m_response.m_success = false;

                   auto response = HttpResponse::newHttpJsonResponse(m_response.to_json());
                response->setStatusCode(k404NotFound);

                return response;
            }

            if (auto image = customer.getImgpath(); image && !image->empty())
                return HttpResponse::newFileResponse(*customer.getImgpath());
        }

        m_response.m_message = "Unable to retreive customers image";
        m_response.m_success = false;

        auto response = HttpResponse::newHttpJsonResponse(m_response.to_json());
        response->setStatusCode(k404NotFound);

        LOG(WARNING) << "Unable to retreive customers image";

        return response;
    }
    HttpResponsePtr customer_service::getThumbnail(HttpRequestPtr const& req, std::string&& id)
    {
        MasterCustomers customer;

        if (id.empty() || !util::is_numeric(id))
        {
            LOG(WARNING) << "ID is empty or ID is not numeric";

            return BadRequestException("Parameters requirement doesn't match").response();
        }

        if (g_customer_manager->find(stoll(id), &customer))
        {
            std::filesystem::path file(*customer.getImgthumbpath());

            if (!std::filesystem::exists(file))
            {
                LOG(WARNING) << "File at " << file.lexically_normal() << " doesn't exist in server";

                m_response.m_message = "Unable to retreive profile picture, please upload your profile picture";
                m_response.m_success = false;

                auto response = HttpResponse::newHttpJsonResponse(m_response.to_json());
                response->setStatusCode(k404NotFound);

                return response;
            }

            if (auto image = customer.getImgpath(); image && !image->empty())
                return HttpResponse::newFileResponse(*customer.getImgthumbpath());
        }

        m_response.m_message = "Unable to retreive customers image";
        m_response.m_success = false;

        auto response = HttpResponse::newHttpJsonResponse(m_response.to_json());
        response->setStatusCode(k404NotFound);

        LOG(WARNING) << "Unable to retreive customers image";

        return response;
    }
}