#include "category.h"

// Add definition of your processing function here
namespace gaboot
{
    void category::create(HttpRequestPtr const& req, response_t&& callback)
    {
        callback(m_category_service.create(req));
    }
    void category::findAll(HttpRequestPtr const& req, response_t&& callback)
    {
        callback(m_category_service.findAll(req));
    }
    void category::findOne(HttpRequestPtr const& req, response_t&& callback, std::string&& id)
    {
        callback(m_category_service.findOne(req, std::move(id)));
    }
    void category::update(HttpRequestPtr const& req, response_t&& callback, std::string&& id)
    {
        callback(m_category_service.update(req, std::move(id)));
    }
    void category::remove(HttpRequestPtr const& req, response_t&& callback, std::string&& id)
    {
        callback(m_category_service.remove(req, std::move(id)));
    }
    void category::getImage(HttpRequestPtr const& req, response_t&& callback, std::string&& id)
    {
        callback(m_category_service.getImage(req, std::move(id)));
    }
    void category::getThumbnail(HttpRequestPtr const& req, response_t&& callback, std::string&& id)
    {
        callback(m_category_service.getThumbnail(req, std::move(id)));
    }
}