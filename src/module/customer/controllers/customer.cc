#include "customer.h"
#include "util/gaboot.hpp"
#include "uploader/upload.hpp"
#include "exception/exception.hpp"
#include "file_manager/file_manager.hpp"

// Add definition of your processing function here
namespace gaboot
{
    void customer::findAll(HttpRequestPtr const& req, response_t&& callback)
    {
        TRY_CLAUSE
        callback(m_customer_service.findAll(req));
        EXCEPT_CLAUSE
    }

    void customer::findOne(HttpRequestPtr const& req, response_t&& callback, std::string&& id)
    {
        TRY_CLAUSE
        callback(m_customer_service.findOne(req, std::move(id)));
        EXCEPT_CLAUSE
    }

    void customer::create(HttpRequestPtr const& req, response_t&& callback)
    {
        TRY_CLAUSE
        callback(m_customer_service.create(req));
        EXCEPT_CLAUSE
    }

    void customer::update(HttpRequestPtr const& req, response_t&& callback, std::string&& id)
    {
        TRY_CLAUSE
        callback(m_customer_service.update(req, std::move(id)));
        EXCEPT_CLAUSE
    }

    void customer::remove(HttpRequestPtr const& req, response_t&& callback, std::string&& id)
    {
        TRY_CLAUSE
        callback(m_customer_service.remove(req, std::move(id)));
        EXCEPT_CLAUSE
    }

    void customer::updateImage(HttpRequestPtr const& req, response_t&& callback, std::string&& id)
    {
        TRY_CLAUSE
        callback(m_customer_service.updateImage(req, std::move(id)));
        EXCEPT_CLAUSE
    }

    void customer::getImage(HttpRequestPtr const& req, response_t&& callback, std::string&& id)
    {
        TRY_CLAUSE
        callback(m_customer_service.getImage(req, std::move(id)));
        EXCEPT_CLAUSE
    }

    void customer::getThumbnail(HttpRequestPtr const& req, response_t&& callback, std::string&& id)
    {
        TRY_CLAUSE
        callback(m_customer_service.getThumbnail(req, std::move(id)));
        EXCEPT_CLAUSE
    }

    void customer::getProfile(HttpRequestPtr const& req, response_t&& callback, std::string&& id)
    {
        TRY_CLAUSE
        callback(m_customer_service.getProfile(req, std::move(id)));
        EXCEPT_CLAUSE
    }
}