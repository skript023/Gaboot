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
	public:
		explicit customer_service();
		~customer_service() noexcept;

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
		std::unique_ptr<response_data<CustomerResponse>> m_response;
		std::unique_ptr<Mapper<MasterCustomers>> m_database;
		std::string m_error;
		Json::Value m_data;
	};
}