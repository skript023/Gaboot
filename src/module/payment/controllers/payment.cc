#include "payment.h"

// Add definition of your processing function here
namespace gaboot
{
	void payment::create(HttpRequestPtr const& req, response_t&& callback)
	{
		callback(m_payment_service.create(req));
	}
	void payment::callback(HttpRequestPtr const& req, response_t&& callback)
	{
		callback(m_payment_service.callback(req));
	}
}