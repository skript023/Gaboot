#pragma once

#include <pch.h>
#include <interfaces/response.hpp>
#include <module/products/models/MasterProducts.h>
#include <module/products/models/ProductImages.h>

using namespace drogon;
using namespace orm;
using namespace drogon_model::gaboot;

namespace gaboot
{
	class product_service
	{
		Mapper<MasterProducts> db() { return Mapper<MasterProducts>(DATABASE_CLIENT); }
		Mapper<ProductImages> db_images() { return Mapper<ProductImages>(DATABASE_CLIENT); }
	public:
		explicit product_service() = default;
		virtual ~product_service() = default;

		product_service(product_service const& that) = delete;
		product_service& operator=(product_service const& that) = delete;

		product_service(product_service&& that) = delete;
		product_service& operator=(product_service&& that) = delete;

		HttpResponsePtr create(HttpRequestPtr const&);
		HttpResponsePtr findAll(HttpRequestPtr const&);
		HttpResponsePtr findOne(HttpRequestPtr const&, std::string&&);
		HttpResponsePtr update(HttpRequestPtr const&, std::string&&);
		HttpResponsePtr remove(HttpRequestPtr const&, std::string&&);
	private:
		response_data m_response;
		std::string m_error;
		Json::Value m_data;
		Json::Value m_data_image;
	};
}