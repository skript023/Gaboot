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
		std::string id;
		std::string name;
		std::string description;
		std::string imagePath;
		std::string thumbnailPath;
		std::string createdAt;
		std::string updatedAt;
		std::vector<CategoryResponse> m_vector;

		Json::Value to_json()
		{
			nlohmann::json json;

			if (m_vector.empty())
			{
				json = *this;
			}
			else
			{
				for (auto& var : m_vector)
				{
					json.emplace_back(var);
				}
			}

			Json::Value data;
			Json::Reader reader;
			
			reader.parse(json.dump(), data);

			return data;
		}

		template<typename U>
		std::enable_if<std::is_same<U, std::vector<Categories>>::value, void>::type operator=(const U& args)
		{
			for (const auto& res : args)
			{
				id = res.getValueOfId();
				name = res.getValueOfName();
				description = res.getValueOfDescription();
				imagePath = res.getValueOfImagePath();
				thumbnailPath = res.getValueOfThumbnailPath();
				createdAt = res.getValueOfCreatedAt().toDbStringLocal();
				updatedAt = res.getValueOfUpdatedAt().toDbStringLocal();

				m_vector.emplace_back(*this);
			}
		}

		template<typename U>
		std::enable_if<std::is_same<U, Categories>::value, void>::type operator=(const U& args)
		{
			id = args.getValueOfId();
			name = args.getValueOfName();
			description = args.getValueOfDescription();
			imagePath = args.getValueOfImagePath();
			thumbnailPath = args.getValueOfThumbnailPath();
			createdAt = args.getValueOfCreatedAt().toDbStringLocal();
			updatedAt = args.getValueOfUpdatedAt().toDbStringLocal();
		}

		template<typename U>
		std::enable_if<std::is_same<U, Categories*>::value, void>::type operator=(U args)
		{
			id = args->getValueOfId();
			name = args->getValueOfName();
			description = args->getValueOfDescription();
			imagePath = args->getValueOfImagePath();
			thumbnailPath = args->getValueOfThumbnailPath();
			createdAt = args->getValueOfCreatedAt().toDbStringLocal();
			updatedAt = args->getValueOfUpdatedAt().toDbStringLocal();
		}

		NLOHMANN_DEFINE_TYPE_INTRUSIVE(CategoryResponse, id, name, description, imagePath, thumbnailPath, createdAt, updatedAt)
	};
}