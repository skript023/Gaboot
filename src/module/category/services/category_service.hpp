#pragma once

#include <pch.h>

#include "util/validator.hpp"
#include "interfaces/response.hpp"
#include "category/models/Categories.h"

#include "cache_handler.hpp"

using namespace drogon;
using namespace orm;
using namespace drogon_model::gaboot;

namespace gaboot
{
	class category_service
	{
		Mapper<Categories> db() { return Mapper<Categories>(DATABASE_CLIENT); }
	public:
		explicit category_service();
		~category_service();

		category_service(category_service const& that) = delete;
		category_service& operator=(category_service const& that) = delete;

		category_service(category_service&& that) = delete;
		category_service& operator=(category_service&& that) = delete;

		HttpResponsePtr create(HttpRequestPtr const&);
		HttpResponsePtr findAll(HttpRequestPtr const&);
		HttpResponsePtr findOne(HttpRequestPtr const&, std::string&& id);
		HttpResponsePtr update(HttpRequestPtr const&, std::string&& id);
		HttpResponsePtr remove(HttpRequestPtr const&, std::string&& id);
		HttpResponsePtr getImage(HttpRequestPtr const&, std::string&&);
		HttpResponsePtr getThumbnail(HttpRequestPtr const&, std::string&&);
	private:
		void load_cache()
		{
			if (m_cache_category.empty() || m_cache_category.expired())
			{
				auto categories = db().orderBy(Categories::Cols::_name).findFutureAll().get();
				m_cache_category.cache_duration(5min);

				std::ranges::for_each(categories.begin(), categories.end(), [this](Categories category) {
					m_cache_category.insert(*category.getId(), &category);
				});
			}
		}
	private:
		response_data m_response;
		std::string m_error;
		Json::Value m_data;
		cache_handler<Categories> m_cache_category;
	};
}