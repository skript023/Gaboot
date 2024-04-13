#pragma once
#include <pch.h>
#include <Categories.h>

using namespace drogon;
using namespace orm;
using namespace drogon_model::gaboot;

namespace gaboot
{
	struct CategoryResponse
	{
		CategoryResponse() = default;

		CategoryResponse(Categories* res) :
			id(res->getValueOfId()),
			name(res->getValueOfName()),
			description(res->getValueOfDescription()),
			imagePath(res->getValueOfImagePath()),
			thumbnailPath(res->getValueOfThumbnailPath()),
			createdAt(res->getValueOfCreatedAt().toDbStringLocal()),
			updatedAt(res->getValueOfUpdatedAt().toDbStringLocal())
		{

		}
		
		CategoryResponse(Categories const& res) :
			id(res.getValueOfId()),
			name(res.getValueOfName()),
			description(res.getValueOfDescription()),
			imagePath(res.getValueOfImagePath()),
			thumbnailPath(res.getValueOfThumbnailPath()),
			createdAt(res.getValueOfCreatedAt().toDbStringLocal()),
			updatedAt(res.getValueOfUpdatedAt().toDbStringLocal())
		{

		}

		std::string id;
		std::string name;
		std::string description;
		std::string imagePath;
		std::string thumbnailPath;
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

		NLOHMANN_DEFINE_TYPE_INTRUSIVE(CategoryResponse, id, name, description, imagePath, thumbnailPath, createdAt, updatedAt)
	};
}