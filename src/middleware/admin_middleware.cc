/**
 *
 *  admin_middleware.cc
 *
 */

#include "admin_middleware.h"
#include "util/exception.hpp"

using namespace drogon;

namespace gaboot
{
    void admin_middleware::doFilter(const HttpRequestPtr& req,
        FilterCallback&& fcb,
        FilterChainCallback&& fccb)
    {
        //Edit your logic here
        if (this->is_admin())
        {
            //Passed
            fccb();
            return;
        }
        //Check failed
        auto res = UnauthorizedException("You're not unable to access this route, please contact administrator if you're an employee").response();
        fcb(res);
    }
}
