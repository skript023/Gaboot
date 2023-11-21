#include "payment.h"

// Add definition of your processing function here
namespace gaboot
{
	void payment::create(HttpRequestPtr const& req, response_t&& callback)
	{
		return callback(m_payment_service.create(req));
	}
}