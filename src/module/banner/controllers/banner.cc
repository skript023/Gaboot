#include "banner.h"
#include "exception/exception.hpp"

// Add definition of your processing function here
namespace gaboot
{
    void banner::findAll(HttpRequestPtr const& req, response_t&& callback)
    {
        TRY_CLAUSE
        callback(m_banner_service.findAll(req));
        EXCEPT_CLAUSE
    }
    void banner::findOne(HttpRequestPtr const& req, response_t&& callback, std::string&& id)
    {
        TRY_CLAUSE
        callback(m_banner_service.findOne(req, std::move(id)));
        EXCEPT_CLAUSE
    }
    void banner::getImage(HttpRequestPtr const& req, response_t&& callback, std::string&& id)
    {
        TRY_CLAUSE
        callback(m_banner_service.getImage(req, std::move(id)));
        EXCEPT_CLAUSE
    }
    void banner::getThumbnail(HttpRequestPtr const& req, response_t&& callback, std::string&& id)
    {
        TRY_CLAUSE
        callback(m_banner_service.getThumbnail(req, std::move(id)));
        EXCEPT_CLAUSE
    }
}