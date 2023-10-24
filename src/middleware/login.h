/**
 *
 *  login.h
 *
 */

#pragma once

#include <drogon/HttpFilter.h>
#include <module/customer/models/MasterCustomers.h>
using namespace drogon;

namespace gaboot
{
    class login : public HttpFilter<login>
    {
    public:
        login() {}
        void doFilter(const HttpRequestPtr& req,
            FilterCallback&& fcb,
            FilterChainCallback&& fccb) override;
    private:
        [[nodiscard]] bool parse_token(std::string const& header);
        [[nodiscard]] std::string verify_token(std::string const& header);
    private:
        std::string m_token;
        inline static constexpr char* m_prefix = "Bearer ";
    };
}
