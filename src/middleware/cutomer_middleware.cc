/**
 *
 *  login.cc
 *
 */

#include <pch.h>
#include "cutomer_middleware.h"
#include "util/exception.hpp"
#include "module/customer/services/customer_manager.hpp"

using namespace drogon;

namespace gaboot
{
    using traits = jwt::traits::nlohmann_json;
    void customer_middleware::doFilter(const HttpRequestPtr& req, FilterCallback&& fcb,  FilterChainCallback&& fccb)
    {
        std::string param = req->getHeader("Authorization");

        try 
        {
            if (this->parse_token_from_header(param))
            {
                auto json = this->verify_token();
                auto id = json["id"].get<int64_t>();
                
                g_customer_manager->insert(id);
                req->setBody(std::to_string(id));

                return fccb();
            }

            return fcb(UnauthorizedException("You're currently not logged in, please login!").response());
        }
        catch (const jwt::error::token_verification_exception& e)
        {
            LOG(WARNING) << e.what();

            auto res = UnauthorizedException(e.what()).response();

            fcb(res);
        }
    }
    bool customer_middleware::parse_token_from_header(std::string const& header)
    {
        if (auto it = header.find(m_prefix); it != std::string::npos)
        {
            m_token = header.substr(sizeof(m_prefix) - 1);

            return true;
        }

        LOG(WARNING) << "Invalid Bearer token string.";

        return false;
    }
    bool customer_middleware::parse_token_from_cookie(std::string const& cookie)
    {
        if (!cookie.empty())
        {
            m_token = cookie;

            return true;
        }

        LOG(WARNING) << "Invalid Bearer token string.";

        return false;
    }
    nlohmann::json customer_middleware::verify_token()
    {
        auto decoded = jwt::decode<traits>(m_token);

        jwt::verify<traits>().
            allow_algorithm(jwt::algorithm::hs256{ SECRET }).
            with_issuer("gaboot").
            verify(decoded);

        return nlohmann::json::parse(decoded.get_payload_claim("gaboot").as_string());
    }
}
