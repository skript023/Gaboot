#include "cart.h"
#include "exception/exception.hpp"

// Add definition of your processing function here
namespace gaboot
{
    void cart::create(HttpRequestPtr const &req, response_t &&callback)
    {
        TRY_CLAUSE
        callback(m_cart_service.create(req));
        EXCEPT_CLAUSE
    }
    void cart::findAll(HttpRequestPtr const &req, response_t &&callback)
    {
        TRY_CLAUSE
        callback(m_cart_service.findAll(req));
        EXCEPT_CLAUSE
    }
    void cart::findOne(HttpRequestPtr const &req, response_t &&callback, std::string &&id)
    {
        TRY_CLAUSE
        callback(m_cart_service.findOne(req, std::move(id)));
        EXCEPT_CLAUSE
    }
    void cart::update(HttpRequestPtr const &req, response_t &&callback, std::string &&id)
    {
        TRY_CLAUSE
        callback(m_cart_service.update(req, std::move(id)));
        EXCEPT_CLAUSE
    }
    void cart::remove(HttpRequestPtr const &req, response_t &&callback, std::string &&id)
    {
        TRY_CLAUSE
        callback(m_cart_service.remove(req, std::move(id)));
        EXCEPT_CLAUSE
    }
}