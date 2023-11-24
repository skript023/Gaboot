#include "customer.h"
#include "util/upload.hpp"
#include "util/gaboot.hpp"
#include "util/exception.hpp"
#include "util/file_manager.hpp"
#include "module/customer/services/customer_manager.hpp"

// Add definition of your processing function here
namespace gaboot
{
    void customer::findAll(HttpRequestPtr const& req, response_t&& callback)
    {
        callback(m_customer_service.findAll(req));
    }

    void customer::findOne(HttpRequestPtr const& req, response_t&& callback, std::string&& id)
    {
        callback(m_customer_service.findOne(req, std::move(id)));
    }

    void customer::create(HttpRequestPtr const& req, response_t&& callback)
    {
        callback(m_customer_service.create(req));
    }

    void customer::update(HttpRequestPtr const& req, response_t&& callback, std::string&& id)
    {
        callback(m_customer_service.update(req, std::move(id)));
    }

    void customer::remove(HttpRequestPtr const& req, response_t&& callback, std::string&& id)
    {
        callback(m_customer_service.remove(req, std::move(id)));
    }

    void customer::getImage(HttpRequestPtr const& req, response_t&& callback, std::string&& id)
    {
        callback(m_customer_service.getImage(req, std::move(id)));
    }

    void customer::getThumbnail(HttpRequestPtr const& req, response_t&& callback, std::string&& id)
    {
        callback(m_customer_service.getThumbnail(req, std::move(id)));
    }

    void customer::getProfile(HttpRequestPtr const& req, response_t&& callback, std::string&& id)
    {
        callback(m_customer_service.getProfile(req, std::move(id)));
    }
}