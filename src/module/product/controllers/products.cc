#include "products.h"
#include "exception/exception.hpp"

// Add definition of your processing function here
namespace gaboot
{
	void products::findAll(HttpRequestPtr const& req, response_t&& callback)
	{
		TRY_CLAUSE
		callback(m_product_service.findAll(req));
		EXCEPT_CLAUSE
	}
	void products::findOne(HttpRequestPtr const& req, response_t&& callback, std::string&& id)
	{
		TRY_CLAUSE
		callback(m_product_service.findOne(req, std::move(id)));
		EXCEPT_CLAUSE
	}
	void products::update(HttpRequestPtr const& req, response_t&& callback, std::string&& id)
	{
		TRY_CLAUSE
		callback(m_product_service.update(req, std::move(id)));
		EXCEPT_CLAUSE
	}
	void products::create(HttpRequestPtr const& req, response_t&& callback)
	{
		TRY_CLAUSE
		callback(m_product_service.create(req));
		EXCEPT_CLAUSE
	}
	void products::remove(HttpRequestPtr const& req, response_t&& callback, std::string&& id)
	{
		TRY_CLAUSE
		callback(m_product_service.remove(req, std::move(id)));
		EXCEPT_CLAUSE
	}
	void products::productWithImages(HttpRequestPtr const& req, response_t&& callback)
	{
		TRY_CLAUSE
		callback(m_product_service.getProductWithImage(req));
		EXCEPT_CLAUSE
	}
}