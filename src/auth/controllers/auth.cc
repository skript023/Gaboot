#include "auth.h"
#include "util/gaboot.hpp"
#include "auth/generate_token.hpp"
#include "customer/services/customer_manager.hpp"
// Add definition of your processing function here
namespace gaboot
{
    void auth::login(HttpRequestPtr const& req, response_t&& callback)
    {
        callback(m_auth_service.login(req));
    }
    void auth::logout(HttpRequestPtr const& req, response_t&& callback)
    {
        callback(m_auth_service.logout(req));
    }
}