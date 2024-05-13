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
		std::string id;
		std::string productId;
		std::string category;
		std::string createdAt;
		std::string updatedAt;
		std::vector<WishlistResponse> m_vector;

		void clear()
		{
			id.clear();
			productId.clear();
			category.clear();
			createdAt.clear();
			updatedAt.clear();
			m_vector.clear();
		}

		bool empty()
		{
			return id.empty() && productId.empty() && category.empty() && createdAt.empty() && updatedAt.empty();
		}

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

			if (!m_vector.empty()) m_vector.clear();

			return data;
		}

		template<typename U>
		std::enable_if<std::is_same<U, std::vector<Wishlists>>::value, void>::type operator=(const U& args)
		{
			for (const auto& res : args)
			{
				id = res.getValueOfId();
				productId = res.getValueOfProductId();
				category = res.getValueOfCategory();
				createdAt = res.getValueOfCreatedAt().toDbStringLocal();
				updatedAt = res.getValueOfUpdatedAt().toDbStringLocal();

				m_vector.emplace_back(*this);
			}
		}

		template<typename U>
		std::enable_if<std::is_same<U, Wishlists>::value, void>::type operator=(const U& args)
		{
			id = args.getValueOfId();
			productId = args.getValueOfProductId();
			category = args.getValueOfCategory();
			createdAt = args.getValueOfCreatedAt().toDbStringLocal();
			updatedAt = args.getValueOfUpdatedAt().toDbStringLocal();
		}

		template<typename U>
		std::enable_if<std::is_same<U, Wishlists*>::value, void>::type operator=(U args)
		{
			id = args->getValueOfId();
			productId = args->getValueOfProductId();
			category = args->getValueOfCategory();
			createdAt = args->getValueOfCreatedAt().toDbStringLocal();
			updatedAt = args->getValueOfUpdatedAt().toDbStringLocal();
		}

		NLOHMANN_DEFINE_TYPE_INTRUSIVE(WishlistResponse, id, productId, category, createdAt, updatedAt)
	};
}