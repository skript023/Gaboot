#pragma once
#include <pch.h>
#include <Orders.h>

using namespace drogon;
using namespace orm;
using namespace drogon_model::gaboot;

namespace gaboot
{
	struct OrderDetail
	{
		std::string id;
		std::string orderId;
		std::string productId;
		std::string price;
		std::string discount;
		std::string quantity;
		std::string total;
		std::string createdAt;
		std::string updatedAt;

		NLOHMANN_DEFINE_TYPE_INTRUSIVE(OrderDetail, id, orderId, productId, price, discount, quantity, total, createdAt, updatedAt)
	};

	struct OrderResponse
	{
		std::string id;
		std::string name;
		std::string customerId;
		double totalPrice;
		double discount;
		double grandTotal;
		int totalItem;
		std::string status;
		std::string expired;
		std::string createdAt;
		std::string updatedAt;
		std::vector<OrderDetail> detail;
		std::vector<OrderResponse> m_vector;

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
		std::enable_if<std::is_same<U, std::vector<Orders>>::value, void>::type operator=(const U& args)
		{
			for (const auto& res : args)
			{
				id = res.getValueOfId();
				name = res.getValueOfName();
				customerId = res.getValueOfCustomerId();
				totalPrice = res.getValueOfTotalPrice();
				discount = res.getValueOfDiscount();
				grandTotal = res.getValueOfGrandTotal();
				totalItem = res.getValueOfTotalItem();
				status = res.getValueOfStatus();
				expired = res.getValueOfExpired();
				createdAt = res.getValueOfCreatedAt().toDbStringLocal();
				updatedAt = res.getValueOfUpdatedAt().toDbStringLocal();

				m_vector.emplace_back(*this);
			}
		}

		template<typename U>
		std::enable_if<std::is_same<U, Orders>::value, void>::type operator=(const U& args)
		{
			id = args.getValueOfId();
			name = args.getValueOfName();
			customerId = args.getValueOfCustomerId();
			totalPrice = args.getValueOfTotalPrice();
			discount = args.getValueOfDiscount();
			grandTotal = args.getValueOfGrandTotal();
			totalItem = args.getValueOfTotalItem();
			status = args.getValueOfStatus();
			expired = args.getValueOfExpired();
			createdAt = args.getValueOfCreatedAt().toDbStringLocal();
			updatedAt = args.getValueOfUpdatedAt().toDbStringLocal();
		}

		template<typename U>
		std::enable_if<std::is_same<U, Orders*>::value, void>::type operator=(U args)
		{
			id = args->getValueOfId();
			name = args->getValueOfName();
			customerId = args->getValueOfCustomerId();
			totalPrice = args->getValueOfTotalPrice();
			discount = args->getValueOfDiscount();
			grandTotal = args->getValueOfGrandTotal();
			totalItem = args->getValueOfTotalItem();
			status = args->getValueOfStatus();
			expired = args->getValueOfExpired();
			createdAt = args->getValueOfCreatedAt().toDbStringLocal();
			updatedAt = args->getValueOfUpdatedAt().toDbStringLocal();
		}

		NLOHMANN_DEFINE_TYPE_INTRUSIVE(OrderResponse, id, name, customerId, totalPrice, discount, grandTotal, totalItem, status, expired, detail, createdAt, updatedAt)
	};
}