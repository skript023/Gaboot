/**
 *
 *  admin_middleware.h
 *
 */

#pragma once

#include <drogon/HttpFilter.h>
using namespace drogon;

namespace gaboot
{
    class admin_middleware : public HttpFilter<admin_middleware>
    {
    public:
        admin_middleware() {}
        void doFilter(const HttpRequestPtr& req,
            FilterCallback&& fcb,
            FilterChainCallback&& fccb) override;
    private:
        [[nodiscard]] bool is_admin() { return m_user; }
    private:
        bool m_user = false;
    };
}

