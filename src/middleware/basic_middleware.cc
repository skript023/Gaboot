/**
 *
 *  admin_middleware.cc
 *
 */

#include "basic_middleware.h"
#include "exception/exception.hpp"

using namespace drogon;

namespace gaboot
{
    void basic_middleware::doFilter(const HttpRequestPtr& req, FilterCallback&& fcb, FilterChainCallback&& fccb)
    {
        try
        {
            std::string param = req->getHeader("Authorization");

            if (this->parse_token_from_header(param))
            {
                auto customers = customer().findBy(Criteria(MasterCustomers::Cols::_token, CompareOperator::EQ, m_token));

                return fccb();
            }

            throw UnauthorizedException("You're unauthorized");
        }
        catch (UnauthorizedException const& ex)
        {
            fcb(ex.response());
        }
    }

    bool basic_middleware::parse_token_from_header(std::string const& header)
    {
        if (auto it = header.find("Basic"); it != std::string::npos)
        {
            m_token = header.substr(sizeof("Basic") - 1);

            return true;
        }

        LOG(WARNING) << "Invalid Basic token string.";

        return false;
    }
}
