#pragma once
#include <pch.h>
#include <cart/models/Carts.h>

using namespace drogon;
using namespace orm;
using namespace drogon_model::gaboot;

namespace gaboot
{
	struct CartsResponse
	{
		CartsResponse() = default;

		CartsResponse(Carts const& res):
			id(res.getValueOfId()), 
			customerId(res.getValueOfCustomerId()),
			productId(res.getValueOfProductId()),
			price(res.getValueOfPrice()),
			quantity(res.getValueOfQuantity()),
			createdAt(res.getValueOfCreatedAt().toDbStringLocal()),
			updatedAt(res.getValueOfUpdatedAt().toDbStringLocal())
		{

		}

		CartsResponse(Carts* res) :
			id(res->getValueOfId()),
			customerId(res->getValueOfCustomerId()),
			productId(res->getValueOfProductId()),
			price(res->getValueOfPrice()),
			quantity(res->getValueOfQuantity()),
			createdAt(res->getValueOfCreatedAt().toDbStringLocal()),
			updatedAt(res->getValueOfUpdatedAt().toDbStringLocal())
		{
			//if (!res) throw NotFoundException("Cart not found");
		}

		std::string id;
		std::string customerId;
		std::string productId;
		double price;
		int32_t quantity;
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
		NLOHMANN_DEFINE_TYPE_INTRUSIVE(CartsResponse, id, customerId, productId, price, quantity, createdAt, updatedAt)
	};
}