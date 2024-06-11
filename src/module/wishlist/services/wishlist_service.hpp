#pragma once

#include <drogon/HttpController.h>
#include "wishlist/models/Wishlists.h"

#include "validator/validator.hpp"
#include "interfaces/response.hpp"
#include "cache_manager/cache_handler.hpp"
#include <dto/wishlist.dto.hpp>


using namespace drogon;
using namespace orm;
using namespace drogon_model::gaboot;

namespace gaboot
{
	class wishlist_service
	{
	public:
		explicit wishlist_service();
		~wishlist_service() noexcept;

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
				auto categories = m_database->orderBy(Wishlists::Cols::_category).findFutureAll().get();
				m_cache_wishlist.cache_duration(5min);

				std::ranges::for_each(categories.begin(), categories.end(), [this](Wishlists wishlist) {
					m_cache_wishlist.insert(*wishlist.getId(), &wishlist);
				});
			}
		}
	private:
		std::unique_ptr<response_data<WishlistResponse>> m_response;
		std::unique_ptr<Mapper<Wishlists>> m_database;
		cache_handler<Wishlists> m_cache_wishlist;
		std::string m_error;
	};
}