#pragma once

#include <pch.h>

#include "validator/validator.hpp"
#include "interfaces/response.hpp"
#include "order/models/Orders.h"
#include <cache_manager/cache_handler.hpp>
#include <dto/order.dto.hpp>

using namespace drogon;
using namespace orm;
using namespace drogon_model::gaboot;

namespace gaboot
{
	class order_service
	{
	public:
		explicit order_service();
		~order_service() noexcept;

		order_service(order_service const& that) = delete;
		order_service& operator=(order_service const& that) = delete;

		order_service(order_service&& that) = delete;
		order_service& operator=(order_service&& that) = delete;

		HttpResponsePtr create(HttpRequestPtr const&);
		HttpResponsePtr findAll(HttpRequestPtr const&);
		HttpResponsePtr findOne(HttpRequestPtr const&, std::string&& id);
		HttpResponsePtr update(HttpRequestPtr const&, std::string&& id);
		HttpResponsePtr remove(HttpRequestPtr const&, std::string&& id);
	private:
		void load_cache()
		{
			if (m_cache_order.empty() || m_cache_order.expired())
			{
				auto categories = m_database->orderBy(Orders::Cols::_name).findAll();
				m_cache_order.cache_duration(5min);

				std::ranges::for_each(categories.begin(), categories.end(), [this](Orders order) {
					m_cache_order.insert(*order.getId(), &order);
				});
			}
		}
	private:
		std::unique_ptr<response_data<OrderResponse>> m_response;
		std::unique_ptr<Mapper<Orders>> m_database;
		cache_handler<Orders> m_cache_order;
		std::string m_error;
	};
}