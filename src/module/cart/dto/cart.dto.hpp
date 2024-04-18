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
		std::string id;
		std::string customerId;
		std::string productId;
		double price;
		int32_t quantity;
		std::string createdAt;
		std::string updatedAt;
		std::vector<CartsResponse> m_vector;

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
		std::enable_if<std::is_same<U, std::vector<Carts>>::value, void>::type operator=(const U& args)
		{
			for (const auto& res : args)
			{
				id = res.getValueOfId();
				customerId = res.getValueOfCustomerId();
				productId = res.getValueOfProductId();
				price = res.getValueOfPrice();
				quantity = res.getValueOfQuantity();
				createdAt = res.getValueOfCreatedAt().toDbStringLocal();
				updatedAt = res.getValueOfUpdatedAt().toDbStringLocal();

				m_vector.emplace_back(*this);
			}
		}

		template<typename U>
		std::enable_if<std::is_same<U, Carts>::value, void>::type operator=(const U& args) 
		{
			id = args.getValueOfId();
			customerId = args.getValueOfCustomerId();
			productId = args.getValueOfProductId();
			price = args.getValueOfPrice();
			quantity = args.getValueOfQuantity();
			createdAt = args.getValueOfCreatedAt().toDbStringLocal();
			updatedAt = args.getValueOfUpdatedAt().toDbStringLocal();
		}
		
		template<typename U>
		std::enable_if<std::is_same<U, Carts*>::value, void>::type operator=(U args) 
		{
			id = args->getValueOfId();
			customerId = args->getValueOfCustomerId();
			productId = args->getValueOfProductId();
			price = args->getValueOfPrice();
			quantity = args->getValueOfQuantity();
			createdAt = args->getValueOfCreatedAt().toDbStringLocal();
			updatedAt = args->getValueOfUpdatedAt().toDbStringLocal();
		}

		NLOHMANN_DEFINE_TYPE_INTRUSIVE(CartsResponse, id, customerId, productId, price, quantity, createdAt, updatedAt)
	};
}