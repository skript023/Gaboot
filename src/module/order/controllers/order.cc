#include "order.h"

// Add definition of your processing function here
namespace gaboot
{
    void order::create(HttpRequestPtr const& req, response_t&& callback)
    {
        callback(m_order_service.create(req));
    }
    void order::findAll(HttpRequestPtr const& req, response_t&& callback)
    {
        callback(m_order_service.findAll(req));
    }
    void order::findOne(HttpRequestPtr const& req, response_t&& callback, std::string&& id)
    {
        callback(m_order_service.findOne(req, std::move(id)));
    }
    void order::update(HttpRequestPtr const& req, response_t&& callback, std::string&& id)
    {
        callback(m_order_service.update(req, std::move(id)));
    }
} // namespace gaboot
