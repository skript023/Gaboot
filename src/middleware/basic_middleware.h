/**
 *
 *  admin_middleware.h
 *
 */

#pragma once

#include <pch.h>
#include <drogon/HttpFilter.h>
#include "module/customer/models/MasterCustomers.h"

using namespace drogon;
using namespace orm;
using namespace drogon_model::gaboot;

namespace gaboot
{
    class basic_middleware : public HttpFilter<basic_middleware>
    {
        Mapper<MasterCustomers> db()
        {
            return Mapper<MasterCustomers>(DATABASE_CLIENT);
        }
    public:
        basic_middleware() = default;
        void doFilter(const HttpRequestPtr& req,
            FilterCallback&& fcb,
            FilterChainCallback&& fccb) override;
    private:
        bool parse_token_from_header(std::string const& header);
    private:
        std::string m_token;
    };
}

