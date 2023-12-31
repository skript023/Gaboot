#pragma once

#include <drogon/HttpController.h>
#include "wishlist/models/Wishlists.h"

#include "validator/validator.hpp"
#include "interfaces/response.hpp"
#include "cache_manager/cache_handler.hpp"


using namespace drogon;
using namespace orm;
using namespace drogon_model::gaboot;

namespace gaboot
{
	class wishlist_service
	{
		Mapper<Wishlists> db() { return Mapper<Wishlists>(DATABASE_CLIENT); }
	public:
		explicit wishlist_service();
		~wishlist_service();

		wishlist_service(wishlist_service const& that) = delete;
		wishlist_service& operator=(wishlist_service const& that) = delete;

		wishlist_service(wishlist_service&& that) = delete;
		wishlist_service& operator=(wishlist_service&& that) = delete;

		HttpResponsePtr create(HttpRequestPtr const&);
		HttpResponsePtr findAll(HttpRequestPtr const&);
		HttpResponsePtr findOne(HttpRequestPtr const&, std::string&&);
		HttpResponsePtr update(HttpRequestPtr const&, std::string&&);
		HttpResponsePtr remove(HttpRequestPtr const&, std::string&&);
	private:
		void load_cache()
		{
			if (m_cache_wishlist.empty() || m_cache_wishlist.expired())
			{
				auto categories = db().orderBy(Wishlists::Cols::_category).findFutureAll().get();
				m_cache_wishlist.cache_duration(5min);

				std::ranges::for_each(categories.begin(), categories.end(), [this](Wishlists wishlist) {
					m_cache_wishlist.insert(*wishlist.getId(), &wishlist);
				});
			}
		}
	private:
		cache_handler<Wishlists> m_cache_wishlist;
		response_data m_response;
		std::string m_error;
	};
}