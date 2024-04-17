#pragma once

#include <drogon/HttpController.h>

#include "validator/validator.hpp"
#include "interfaces/response.hpp"
#include "module/customer/models/MasterCustomers.h"
#include <dto/customer.dto.hpp>

using namespace drogon;
using namespace orm;
using namespace drogon_model::gaboot;

namespace gaboot
{
	class customer_service
	{
		Mapper<MasterCustomers> db()
		{
			return Mapper<MasterCustomers>(DATABASE_CLIENT);
		}
	public:
		explicit customer_service() = default;
		virtual ~customer_service() = default;

		customer_service(customer_service const& that) = delete;
		customer_service& operator=(customer_service const& that) = delete;

		customer_service(customer_service&& that) = delete;
		customer_service& operator=(customer_service&& that) = delete;

		HttpResponsePtr findAll(HttpRequestPtr const&);
		HttpResponsePtr create(HttpRequestPtr const&);
		HttpResponsePtr findOne(HttpRequestPtr const&, std::string&&);
		HttpResponsePtr update(HttpRequestPtr const&, std::string&&);
		HttpResponsePtr remove(HttpRequestPtr const&, std::string&&);
		HttpResponsePtr updateImage(HttpRequestPtr const& req, std::string&&);
		HttpResponsePtr getProfile(HttpRequestPtr const&, std::string&&);
		HttpResponsePtr getImage(HttpRequestPtr const&, std::string&&);
		HttpResponsePtr getThumbnail(HttpRequestPtr const&, std::string&&);
	private:
		response_data<CustomerResponse> m_response;
		std::string m_error;
		Json::Value m_data;
	};
}