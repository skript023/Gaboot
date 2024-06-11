#include "order.h"
#include "exception/exception.hpp"

// Add definition of your processing function here
namespace gaboot
{
    void order::create(HttpRequestPtr const& req, response_t&& callback)
    {
        TRY_CLAUSE
        callback(m_order_service.create(req));
        EXCEPT_CLAUSE
    }
    void order::findAll(HttpRequestPtr const& req, response_t&& callback)
    {
        TRY_CLAUSE
        callback(m_order_service.findAll(req));
        EXCEPT_CLAUSE
    }
    void order::findOne(HttpRequestPtr const& req, response_t&& callback, std::string&& id)
    {
        TRY_CLAUSE
        callback(m_order_service.findOne(req, std::move(id)));
        EXCEPT_CLAUSE
    }
    void order::update(HttpRequestPtr const& req, response_t&& callback, std::string&& id)
    {
        TRY_CLAUSE
        callback(m_order_service.update(req, std::move(id)));
        EXCEPT_CLAUSE
    }
} // namespace gaboot
