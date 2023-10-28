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
        [[nodiscard]] bool parse_token(std::string const& header);
        [[nodiscard]] nlohmann::json::object_t verify_token(std::string const& header);
    private:
        std::string m_token;
        std::mutex m_lock;
        std::condition_variable m_data_condition;
        inline static constexpr const char* m_prefix = "Bearer ";
    };
}
