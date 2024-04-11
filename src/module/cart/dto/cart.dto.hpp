#pragma once
#include <pch.h>

namespace gaboot
{
	struct ActualCartsResponse
	{
		std::string id;
		std::string customer_id;
		std::string product_id;
		double price;
		int32_t quantity;
		std::string created_at;
		std::string updated_at;

		NLOHMANN_DEFINE_TYPE_INTRUSIVE(ActualCartsResponse, id, customer_id, product_id, price, quantity, created_at, updated_at)
	};

	struct CartsResponse
	{
		CartsResponse() = default;

		CartsResponse(std::unique_ptr<ActualCartsResponse> const& res):
			id(res->id), 
			customerId(res->customer_id),
			productId(res->product_id),
			price(res->price),
			quantity(res->quantity),
			createdAt(res->created_at),
			updatedAt(res->updated_at)
		{

		}

		std::string id;
		std::string customerId;
		std::string productId;
		double price;
		int32_t quantity;
		std::string createdAt;
		std::string updatedAt;

		CartsResponse from_json(Json::Value const& json)
		{
			auto njson = nlohmann::json::parse(json.toStyledString());

			const auto response = std::make_unique<ActualCartsResponse>(njson.get<ActualCartsResponse>());

			*this = response;

			return *this;
		}

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