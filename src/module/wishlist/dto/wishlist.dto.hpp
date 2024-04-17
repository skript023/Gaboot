#pragma once
#include <pch.h>
#include <Wishlists.h>

using namespace drogon;
using namespace orm;
using namespace drogon_model::gaboot;

namespace gaboot
{
	struct WishlistResponse
	{
		WishlistResponse() = default;

		WishlistResponse(Wishlists* res) :
			id(res->getValueOfId()),
			productId(res->getValueOfProductId()),
			category(res->getValueOfCategory()),
			createdAt(res->getValueOfCreatedAt().toDbStringLocal()),
			updatedAt(res->getValueOfUpdatedAt().toDbStringLocal())
		{

		}

		WishlistResponse(Wishlists const& res) :
			id(res.getValueOfId()),
			productId(res.getValueOfProductId()),
			category(res.getValueOfCategory()),
			createdAt(res.getValueOfCreatedAt().toDbStringLocal()),
			updatedAt(res.getValueOfUpdatedAt().toDbStringLocal())
		{

		}

		std::string id;
		std::string productId;
		std::string category;
		std::string createdAt;
		std::string updatedAt;

		Json::Value to_json()
		{
			nlohmann::json json = *this;
			Json::Value data;
			Json::Reader reader;

			reader.parse(json.dump(), data);

			return data;
		}

		NLOHMANN_DEFINE_TYPE_INTRUSIVE(WishlistResponse, id, productId, category, createdAt, updatedAt)
	};
}