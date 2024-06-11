#include "wishlist.h"
#include "exception/exception.hpp"

// Add definition of your processing function here
namespace gaboot
{
	void wishlist::create(HttpRequestPtr const& req, response_t&& callback)
	{
		TRY_CLAUSE
		callback(m_wishlist_service.create(req));
		EXCEPT_CLAUSE
	}
	void wishlist::findAll(HttpRequestPtr const& req, response_t&& callback)
	{
		TRY_CLAUSE
		callback(m_wishlist_service.findAll(req));
		EXCEPT_CLAUSE
	}
	void wishlist::findOne(HttpRequestPtr const& req, response_t&& callback, std::string&& id)
	{
		TRY_CLAUSE
		callback(m_wishlist_service.findOne(req, std::move(id)));
		EXCEPT_CLAUSE
	}
	void wishlist::update(HttpRequestPtr const& req, response_t&& callback, std::string&& id)
	{
		TRY_CLAUSE
		callback(m_wishlist_service.update(req, std::move(id)));
		EXCEPT_CLAUSE
	}
	void wishlist::remove(HttpRequestPtr const& req, response_t&& callback, std::string&& id)
	{
		TRY_CLAUSE
		callback(m_wishlist_service.remove(req, std::move(id)));
		EXCEPT_CLAUSE
	}
}