#include "payment.h"
#include "exception/exception.hpp"

// Add definition of your processing function here
namespace gaboot
{
	void payment::create(HttpRequestPtr const& req, response_t&& callback)
	{
		TRY_CLAUSE
		callback(m_payment_service.create(req)); 
		EXCEPT_CLAUSE
	}
    void payment::findOne(HttpRequestPtr const &req, response_t &&callback, std::string&& id)
    {
		TRY_CLAUSE
		callback(m_payment_service.findOne(req, std::move(id)));
		EXCEPT_CLAUSE
    }
    void payment::callback(HttpRequestPtr const &req, response_t &&callback)
    {
		TRY_CLAUSE
		callback(m_payment_service.callback(req));
		EXCEPT_CLAUSE
	}
}