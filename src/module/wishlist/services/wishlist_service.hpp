#pragma once

#include <drogon/HttpController.h>

#include "util/validator.hpp"
#include "interfaces/response.hpp"
#include "module/wishlist/models/Wishlists.h"

using namespace drogon;
using namespace orm;
using namespace drogon_model::gaboot;

namespace gaboot
{
	class wishlist_service
	{
		Mapper<Wishlists> db() { return Mapper<Wishlists>(DATABASE_CLIENT); }
	public:
		explicit wishlist_service() = default;
		virtual ~wishlist_service() = default;

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
		response_data m_response;
		std::string m_error;
	};
}