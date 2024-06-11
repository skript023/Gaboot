#include "category.h"
#include "exception/exception.hpp"

// Add definition of your processing function here
namespace gaboot
{
    void category::create(HttpRequestPtr const& req, response_t&& callback)
    {
        TRY_CLAUSE
        callback(m_category_service.create(req));
        EXCEPT_CLAUSE
    }
    void category::findAll(HttpRequestPtr const& req, response_t&& callback)
    {
        TRY_CLAUSE
        callback(m_category_service.findAll(req));
        EXCEPT_CLAUSE
    }
    void category::findOne(HttpRequestPtr const& req, response_t&& callback, std::string&& id)
    {
        TRY_CLAUSE
        callback(m_category_service.findOne(req, std::move(id)));
        EXCEPT_CLAUSE
    }
    void category::update(HttpRequestPtr const& req, response_t&& callback, std::string&& id)
    {
        TRY_CLAUSE
        callback(m_category_service.update(req, std::move(id)));
        EXCEPT_CLAUSE
    }
    void category::remove(HttpRequestPtr const& req, response_t&& callback, std::string&& id)
    {
        TRY_CLAUSE
        callback(m_category_service.remove(req, std::move(id)));
        EXCEPT_CLAUSE
    }
    void category::getImage(HttpRequestPtr const& req, response_t&& callback, std::string&& id)
    {
        TRY_CLAUSE
        callback(m_category_service.getImage(req, std::move(id)));
        EXCEPT_CLAUSE
    }
    void category::getThumbnail(HttpRequestPtr const& req, response_t&& callback, std::string&& id)
    {
        TRY_CLAUSE
        callback(m_category_service.getThumbnail(req, std::move(id)));
        EXCEPT_CLAUSE
    }
}