/**
 *
 *  login.h
 *
 */

#pragma once

#include <drogon/HttpFilter.h>

using namespace drogon;
using namespace orm;

namespace gaboot
{
    class customer_middleware : public HttpFilter<customer_middleware>
    {
    public:
        customer_middleware() {}
        void doFilter(const HttpRequestPtr& req,
            FilterCallback&& fcb,
            FilterChainCallback&& fccb) override;
    private:
        [[nodiscard]] bool parse_token_from_header(std::string const& header);
        [[nodiscard]] bool parse_token_from_cookie(std::string const& header);
        [[nodiscard]] nlohmann::json verify_token();
    private:
        std::string m_token;
        inline static constexpr const char* m_prefix = "Bearer";
    };
}
