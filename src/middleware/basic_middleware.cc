/**
 *
 *  admin_middleware.cc
 *
 */

#include "basic_middleware.h"
#include "exception/exception.hpp"
#include "auth/cache/auth_manager.hpp"

using namespace drogon;

namespace gaboot
{
    void basic_middleware::doFilter(const HttpRequestPtr& req, FilterCallback&& fcb, FilterChainCallback&& fccb)
    {
        try
        {
            std::string param = req->getHeader("Authorization");

            if (!this->parse_token_from_header(param))
            {
                throw UnauthorizedException("You're unauthorized");
            }

            LOG(INFO) << m_token;

            auto customer = db().findOne(Criteria(MasterCustomers::Cols::_token, CompareOperator::EQ, m_token));

            g_auth_manager->insert(&customer);

            return fccb();
        }
        catch (GabootException const& ex)
        {
            fcb(ex.response());
        }
        catch (DrogonDbException const& ex)
        {
            fcb(UnauthorizedException("Token Not found").response());
        }
    }

    bool basic_middleware::parse_token_from_header(std::string const& header)
    {
        if (auto it = header.find("Basic"); it != std::string::npos)
        {   
            m_token = header.substr(sizeof("Basic"));

            return true;
        }

        LOG(WARNING) << "Invalid Basic token string.";

        return false;
    }
}
