#pragma once

#include <pch.h>
#include <interfaces/response.hpp>
#include <module/product/models/MasterProducts.h>
#include <module/product/models/ProductImages.h>

#include "cache_handler.hpp"

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
		explicit product_service();
		~product_service();

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
		void load_cache()
		{
			if (m_cache_product.empty() || m_cache_product.expired())
			{
				auto categories = db().orderBy(MasterProducts::Cols::_name).findFutureAll().get();
				m_cache_product.cache_duration(5min);

				std::ranges::for_each(categories.begin(), categories.end(), [this](MasterProducts wishlist) {
					m_cache_product.insert(*wishlist.getId(), &wishlist);
				});
			}
		}
	private:
		cache_handler<MasterProducts> m_cache_product;
		response_data m_response;
		std::string m_error;
		Json::Value m_data;
		Json::Value m_data_image;
	};
}