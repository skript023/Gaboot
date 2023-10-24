/**
 *
 *  login.cc
 *
 */

#include <pch.h>
#include "login.h"
#include "util/exception.hpp"

using namespace drogon;

namespace gaboot
{
    using traits = jwt::traits::nlohmann_json;
    void login::doFilter(const HttpRequestPtr& req,
        FilterCallback&& fcb,
        FilterChainCallback&& fccb)
    {
        std::string param = req->getHeader("Authorization");

        try
        {
            auto& claim = this->verify_token(param);

            auto& json = nlohmann::json(claim);

            fccb();

            return;
        }
        catch (const std::exception& e)
        {
            LOG(WARNING) << e.what();

            auto res = UnauthorzedException().response();
            fcb(res);
        }
    }
    bool login::parse_token(std::string const& header)
    {
        if (auto it = header.find(m_prefix); it != std::string::npos)
        {
            m_token = header.substr(sizeof(m_prefix) - 1);

            return true;
        }

        LOG(INFO) << "Invalid Bearer token string.";

        return false;
    }
    std::string login::verify_token(std::string const& header)
    {
        auto token = this->parse_token(header);

        if (!token)
        {
            auto decoded = jwt::decode<traits>(m_token);

            auto& verifier = jwt::verify<traits>()
                .allow_algorithm(jwt::algorithm::hs256{ SECRET })
                .with_issuer("jwt");

            verifier.verify(decoded);

            return decoded.get_payload_claim("gaboot").as_string();
        }

        return {};
    }
}
