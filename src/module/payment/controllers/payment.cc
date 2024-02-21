#include "payment.h"

// Add definition of your processing function here
namespace gaboot
{
	void payment::create(HttpRequestPtr const& req, response_t&& callback)
	{
		callback(m_payment_service.create(req));
	}
    void payment::findOne(HttpRequestPtr const &req, response_t &&callback, std::string&& id)
    {
		callback(m_payment_service.findOne(req, std::move(id)));
    }
    void payment::callback(HttpRequestPtr const &req, response_t &&callback)
    {
		callback(m_payment_service.callback(req));
	}
}