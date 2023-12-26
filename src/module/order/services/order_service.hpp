#pragma once

#include <pch.h>

#include "validator/validator.hpp"
#include "interfaces/response.hpp"
#include "order/models/Orders.h"

using namespace drogon;
using namespace orm;
using namespace drogon_model::gaboot;

namespace gaboot
{
	class order_service
	{
		Mapper<Orders> db() { return Mapper<Orders>(DATABASE_CLIENT); }
	public:
		explicit order_service() = default;
		~order_service() noexcept = default;

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
		response_data m_response;
		std::string m_error;
		Json::Value m_data;
	};
}