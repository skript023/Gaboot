#pragma once
#include <pch.h>

#include "validator/validator.hpp"
#include "interfaces/response.hpp"
#include "cart/models/Carts.h"

#include <cache_manager/cache_handler.hpp>

using namespace drogon;
using namespace orm;
using namespace drogon_model::gaboot;

namespace gaboot
{
	class cart_service
	{
		Mapper<Carts> db() { return Mapper<Carts>(DATABASE_CLIENT); }
	public:
		explicit cart_service();
		~cart_service() noexcept;

		cart_service(cart_service const& that) = delete;
		cart_service& operator=(cart_service const& that) = delete;

		cart_service(cart_service&& that) = delete;
		cart_service& operator=(cart_service&& that) = delete;

		HttpResponsePtr create(HttpRequestPtr const& req);
		HttpResponsePtr findAll(HttpRequestPtr const& req);
		HttpResponsePtr findOne(HttpRequestPtr const& req, std::string&& id);
		HttpResponsePtr update(HttpRequestPtr const& req, std::string&& id);
		HttpResponsePtr remove(HttpRequestPtr const& req, std::string&& id);
	private:
		void load_cache()
		{
			if (m_cache_cart.empty() || m_cache_cart.expired())
			{
				auto carts = db().orderBy(Carts::Cols::_customerid).findAll();
				m_cache_cart.cache_duration(5min);

				std::ranges::for_each(carts.begin(), carts.end(), [this](Carts cart) {
					m_cache_cart.insert(*cart.getId(), &cart);
				});
			}
		}
	private:
		cache_handler<Carts> m_cache_cart;
		response_data m_response;
	};
}