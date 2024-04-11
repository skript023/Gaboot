#pragma once
#include <pch.h>

namespace gaboot
{
	struct ActualCategoryResponse
	{
		std::string id;
		std::string name;
		std::string description;
		std::string image_path;
		std::string thumbnail_path;
		std::string created_at;
		std::string updated_at;

		NLOHMANN_DEFINE_TYPE_INTRUSIVE(ActualCategoryResponse, id, name, description, image_path, thumbnail_path, created_at, updated_at)
	};

	struct CategoryResponse
	{
		CategoryResponse() = default;

		CategoryResponse(std::unique_ptr<ActualCategoryResponse> const& res):
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
		std::string name;
		std::string description;
		std::string imagePath;
		std::string thumbnailPath;
		std::string createdAt;
		std::string updatedAt;

		CategoryResponse from_json(Json::Value const& json)
		{
			auto njson = nlohmann::json::parse(json.toStyledString());

			const auto response = std::make_unique<ActualCategoryResponse>(njson.get<ActualCategoryResponse>());

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
		NLOHMANN_DEFINE_TYPE_INTRUSIVE(CategoryResponse, id, name, description, imagePath, thumbnailPath, createdAt, updatedAt)
	};
}