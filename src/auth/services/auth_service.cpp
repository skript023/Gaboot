#include "auth_service.hpp"
#include "util/gaboot.hpp"
#include "auth/generate_token.hpp"
#include "customer/services/customer_manager.hpp"

namespace gaboot
{
    HttpResponsePtr auth_service::login(HttpRequestPtr const& req)
	{
        auto& json = req->getJsonObject();

        std::string username = (*json)["username"].asString();
        std::string password = (*json)["password"].asString();

        try
        {
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

                g_customer_manager->insert(*user.getId(), &user);

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
    HttpResponsePtr auth_service::logout(HttpRequestPtr const& req)
    {
        std::string token = req->getHeader("Authorization");

        if (auto body = req->getBody(); util::is_numeric(body))
        {
            if (g_customer_manager->remove(std::stoi(body.data())))
            {
                token = jwt_generator::parse_token(token);
                jwt_generator::remove(token);

                m_response.m_message = "Logout success";
                m_response.m_success = true;

                auto response = HttpResponse::newHttpJsonResponse(m_response.to_json());

                return response;
            }
        }

        m_response.m_message = "Logout failed";
        m_response.m_success = false;

        auto response = HttpResponse::newHttpJsonResponse(m_response.to_json());
        response->setStatusCode(HttpStatusCode::k401Unauthorized);

        return response;
    }
}