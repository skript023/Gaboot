#include "auth_service.hpp"
#include "util/gaboot.hpp"
#include "hash/sha256.hpp"
#include "auth/generate_token.hpp"
#include "auth/cache/auth_manager.hpp"

namespace gaboot
{
    HttpResponsePtr auth_service::login(HttpRequestPtr const& req)
	{
        try
        {
            auto& json = req->getJsonObject();

            std::string username = (*json)["username"].asString();
            std::string password = (*json)["password"].asString();

            MasterCustomers user = db().findOne(Criteria(MasterCustomers::Cols::_username, CompareOperator::EQ, username));

            if (auto valid = bcrypt::validatePassword(password, *user.getPassword()); valid && !user.getPassword()->empty())
            {
                jwt_generator jwt({
                    {"id", user.getValueOfId()},
                    {"username", user.getValueOfUsername()}
                });

                m_response.m_message = "Login Success";
                m_response.m_success = true;
                m_response.m_data = jwt.result();

                g_auth_manager->insert(*user.getId(), &user);

                auto response = HttpResponse::newHttpJsonResponse(m_response.to_json());

                return response;
            }
            else
            {
                m_response.m_message = "Login failed, credentials doesn't invalid";
                m_response.m_success = false;

                auto response = HttpResponse::newHttpJsonResponse(m_response.to_json());
                response->setStatusCode(HttpStatusCode::k401Unauthorized);

                return response;
            }
        }
        catch (const std::exception& e)
        {
            m_response.m_message = "Login failed, credentials doesn't invalid";
            m_response.m_success = false;

            auto response = HttpResponse::newHttpJsonResponse(m_response.to_json());
            response->setStatusCode(HttpStatusCode::k401Unauthorized);

            return response;
        }
	}
    HttpResponsePtr auth_service::basic_login(HttpRequestPtr const& req)
	{
        try
        {
            auto& json = req->getJsonObject();

            std::string username = (*json)["username"].asString();
            std::string password = (*json)["password"].asString();

            MasterCustomers user = db().findOne(Criteria(MasterCustomers::Cols::_username, CompareOperator::EQ, username));

            if (auto valid = bcrypt::validatePassword(password, *user.getPassword()); valid && !user.getPassword()->empty())
            {
                std::string salt = sha256::encode(fmt::format("{}:{}", username, password) + SECRET + std::to_string(rand()));
                auto token = sha256::encode(salt);

                m_response.m_data.success = true;
                m_response.m_data.message = "Token successfully generated";
                m_response.m_data.token = token;
                
                user.updateByJson(m_response.m_data.token);

                db().update(user);

                g_auth_manager->insert(*user.getId(), &user);

                m_response.m_message = "Login Success";
                m_response.m_success = true;

                g_auth_manager->insert(*user.getId(), &user);

                auto response = HttpResponse::newHttpJsonResponse(m_response.to_json());

                return response;
            }
            else
            {
                m_response.m_message = "Login failed, credentials doesn't invalid";
                m_response.m_success = false;

                auto response = HttpResponse::newHttpJsonResponse(m_response.to_json());
                response->setStatusCode(HttpStatusCode::k401Unauthorized);

                return response;
            }
        }
        catch (const std::exception& e)
        {
            m_response.m_message = "Login failed, credentials doesn't invalid";
            m_response.m_success = false;

            auto response = HttpResponse::newHttpJsonResponse(m_response.to_json());
            response->setStatusCode(HttpStatusCode::k401Unauthorized);

            return response;
        }
	}
    HttpResponsePtr auth_service::basic_logout(HttpRequestPtr const& req)
    {
        std::string token = req->getHeader("Authorization");

        if (auto body = req->getBody(); util::is_numeric(body) && g_auth_manager->remove(body.data()))
        {
            token = jwt_generator::parse_token(token);
            jwt_generator::remove(token);

            m_response.m_message = "Logout success";
            m_response.m_success = true;

            auto response = HttpResponse::newHttpJsonResponse(m_response.to_json());

            return response;
        }

        m_response.m_message = "Logout failed";
        m_response.m_success = false;

        auto response = HttpResponse::newHttpJsonResponse(m_response.to_json());
        response->setStatusCode(HttpStatusCode::k401Unauthorized);

        return response;
    }
    HttpResponsePtr auth_service::logout(HttpRequestPtr const& req)
    {
        std::string token = req->getHeader("Authorization");

        if (auto body = req->getBody(); util::is_numeric(body) && g_auth_manager->remove(body.data()))
        {
            token = jwt_generator::parse_token(token);
            jwt_generator::remove(token);

            m_response.m_message = "Logout success";
            m_response.m_success = true;

            auto response = HttpResponse::newHttpJsonResponse(m_response.to_json());

            return response;
        }

        m_response.m_message = "Logout failed";
        m_response.m_success = false;

        auto response = HttpResponse::newHttpJsonResponse(m_response.to_json());
        response->setStatusCode(HttpStatusCode::k401Unauthorized);

        return response;
    }
}