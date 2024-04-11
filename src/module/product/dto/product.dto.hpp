#pragma once
#include <pch.h>

namespace gaboot
{
	struct ActualProductResponse
	{
		std::string id;
		std::string name;
		std::string description;
		double price;
		int32_t stock;
		int32_t dimension;
		int32_t weight;
		int32_t weight_unit;
		int32_t category_id;
		int32_t total_sales;
		int32_t is_active;
		std::string created_at;
		std::string updated_at;

		NLOHMANN_DEFINE_TYPE_INTRUSIVE(ActualProductResponse, id, name, description, price, stock, dimension, weight, weight_unit, category_id, total_sales, is_active, created_at, updated_at)
	};

	struct ProductResponse
	{
		ProductResponse() = default;

		ProductResponse(std::unique_ptr<ActualProductResponse> const& res):
			id(res->id), 
			name(res->name),
			description(res->description),
			price(res->price),
			stock(res->stock),
			dimension(res->dimension),
			weight(res->weight),
			weightUnit(res->weight_unit),
			categoryId(res->category_id),
			totalSales(res->total_sales),
			isActive(res->is_active),
			createdAt(res->created_at),
			updatedAt(res->updated_at)
		{

		}

		std::string id;
		std::string name;
		std::string description;
		double price;
		int32_t stock;
		int32_t dimension;
		int32_t weight;
		int32_t weightUnit;
		int32_t categoryId;
		int32_t totalSales;
		int32_t isActive;
		std::string createdAt;
		std::string updatedAt;

		ProductResponse from_json(Json::Value const& json)
		{
			auto njson = nlohmann::json::parse(json.toStyledString());

			const auto response = std::make_unique<ActualProductResponse>(njson.get<ActualProductResponse>());

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
		NLOHMANN_DEFINE_TYPE_INTRUSIVE(ProductResponse, id, name, description, price, stock, dimension, weight, weightUnit, categoryId, totalSales, isActive, createdAt, updatedAt)
	};
}