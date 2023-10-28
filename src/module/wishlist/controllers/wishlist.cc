#include "wishlist.h"

// Add definition of your processing function here
namespace gaboot
{
	void wishlist::create(HttpRequestPtr const& req, response_t&& callback)
	{
		callback(m_wishlist_service.create(req));
	}
	void wishlist::findAll(HttpRequestPtr const& req, response_t&& callback)
	{
		callback(m_wishlist_service.findAll(req));
	}
	void wishlist::findOne(HttpRequestPtr const& req, response_t&& callback, std::string&& id)
	{
		callback(m_wishlist_service.findOne(req, std::move(id)));
	}
	void wishlist::update(HttpRequestPtr const& req, response_t&& callback, std::string&& id)
	{
		callback(m_wishlist_service.update(req, std::move(id)));
	}
	void wishlist::remove(HttpRequestPtr const& req, response_t&& callback, std::string&& id)
	{
		callback(m_wishlist_service.remove(req, std::move(id)));
	}
}