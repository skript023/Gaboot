#include "cart.h"

// Add definition of your processing function here
namespace gaboot
{
    void cart::create(HttpRequestPtr const &req, response_t &&callback)
    {
        callback(m_cart_service.create(req));
    }
    void cart::findAll(HttpRequestPtr const &req, response_t &&callback)
    {
        callback(m_cart_service.findAll(req));
    }
    void cart::findOne(HttpRequestPtr const &req, response_t &&callback, std::string &&id)
    {
        callback(m_cart_service.findOne(req, std::move(id)));
    }
    void cart::update(HttpRequestPtr const &req, response_t &&callback, std::string &&id)
    {
        callback(m_cart_service.update(req, std::move(id)));
    }
    void cart::remove(HttpRequestPtr const &req, response_t &&callback, std::string &&id)
    {
        callback(m_cart_service.remove(req, std::move(id)));
    }
}