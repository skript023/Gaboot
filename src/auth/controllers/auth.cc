#include "auth.h"
#include "util/gaboot.hpp"
#include "auth/generate_token.hpp"
// Add definition of your processing function here
namespace gaboot
{
    void auth::login(HttpRequestPtr const& req, response_t&& callback)
    {
        Json::Value resp;
        auto& json = req->getJsonObject();

        std::string username = (*json)["username"].asString();
        std::string password = (*json)["password"].asString();

        try
        {
            auto user = db().findOne(Criteria(MasterCustomers::Cols::_username, CompareOperator::EQ, username));
            auto user_password = user.getValueOfPassword();

            if (auto valid = bcrypt::validatePassword(password, user_password); valid && !user_password.empty())
            {
                jwt_create jwt = nlohmann::json({
                    {"id", user.getValueOfId()},
                    {"username", user.getValueOfUsername()}
                });

                resp["message"] = "Login Success";
                resp["success"] = true;
                resp["token"] = jwt.get_token();

                auto response = HttpResponse::newHttpJsonResponse(resp);

                callback(response);
            }
            else
            {
                resp["message"] = "Login failed, credentials doesn't invalid";
                resp["success"] = false;
                resp["token"] = 0;

                auto response = HttpResponse::newHttpJsonResponse(resp);
                response->setStatusCode(HttpStatusCode::k401Unauthorized);

                callback(response);
            }
        }
        catch(const std::exception& e)
        {
            resp["message"] = "Login failed, credentials doesn't invalid";
            resp["success"] = false;
            resp["token"] = 0;

            auto response = HttpResponse::newHttpJsonResponse(resp);
            response->setStatusCode(HttpStatusCode::k401Unauthorized);

            callback(response);
        }
    }
}