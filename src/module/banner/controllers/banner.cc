#include "banner.h"

// Add definition of your processing function here
namespace gaboot
{
    void banner::findAll(HttpRequestPtr const& req, response_t&& callback)
    {
        callback(m_banner_service.findAll(req));
    }
    void banner::findOne(HttpRequestPtr const& req, response_t&& callback, std::string&& id)
    {
        callback(m_banner_service.findOne(req, std::move(id)));
    }
    void banner::getImage(HttpRequestPtr const& req, response_t&& callback, std::string&& id)
    {
        callback(m_banner_service.getImage(req, std::move(id)));
    }
    void banner::getThumbnail(HttpRequestPtr const& req, response_t&& callback, std::string&& id)
    {
        callback(m_banner_service.getThumbnail(req, std::move(id)));
    }
}