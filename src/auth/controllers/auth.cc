#include "auth.h"
#include "util/gaboot.hpp"
#include "auth/generate_token.hpp"
// Add definition of your processing function here
namespace gaboot
{
    void auth::login(HttpRequestPtr const& req, response_t&& callback)
    {
        callback(m_auth_service.basic_login(req));
    }
    void auth::logout(HttpRequestPtr const& req, response_t&& callback)
    {
        callback(m_auth_service.basic_login(req));
    }
}