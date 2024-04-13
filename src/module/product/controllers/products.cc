#include "products.h"

// Add definition of your processing function here
namespace gaboot
{
	void products::findAll(HttpRequestPtr const& req, response_t&& callback)
	{
		callback(m_product_service.findAll(req));
	}
	void products::findOne(HttpRequestPtr const& req, response_t&& callback, std::string&& id)
	{
		callback(m_product_service.findOne(req, std::move(id)));
	}
	void products::update(HttpRequestPtr const& req, response_t&& callback, std::string&& id)
	{
		callback(m_product_service.update(req, std::move(id)));
	}
	void products::create(HttpRequestPtr const& req, response_t&& callback)
	{
		callback(m_product_service.create(req));
	}
	void products::remove(HttpRequestPtr const& req, response_t&& callback, std::string&& id)
	{
		callback(m_product_service.remove(req, std::move(id)));
	}
	void products::productWithImages(HttpRequestPtr const& req, response_t&& callback)
	{
		callback(m_product_service.getProductWithImage(req));
	}
}