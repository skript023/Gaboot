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
		OrderResponse() = default;

		OrderResponse(Orders* res) :
			id(res->getValueOfId()),
			name(res->getValueOfName()),
			customerId(res->getValueOfCustomerId()),
			totalPrice(res->getTotalPrice()),
			discount(res->getValueOfDiscount()),
			grandTotal(res->getValueOfGrandTotal()),
			totalItem(res->getTotalItem()),
			status(res->getValueOfStatus()),
			expired(res->getValueOfExpired()),
			createdAt(res->getValueOfCreatedAt().toDbStringLocal()),
			updatedAt(res->getValueOfUpdatedAt().toDbStringLocal())
		{

		}

		OrderResponse(Orders const& res) :
			id(res.getValueOfId()),
			name(res.getValueOfName()),
			customerId(res.getValueOfCustomerId()),
			totalPrice(res.getTotalPrice()),
			discount(res.getValueOfDiscount()),
			grandTotal(res.getValueOfGrandTotal()),
			totalItem(res.getTotalItem()),
			status(res.getValueOfStatus()),
			expired(res.getValueOfExpired()),
			createdAt(res.getValueOfCreatedAt().toDbStringLocal()),
			updatedAt(res.getValueOfUpdatedAt().toDbStringLocal())
		{

		}

		std::string id;
		std::string name;
		std::string customerId;
		std::string totalPrice;
		std::string discount;
		std::string grandTotal;
		std::string totalItem;
		std::string status;
		std::string expired;
		std::string createdAt;
		std::string updatedAt;
		std::vector<OrderDetail> detail;

		Json::Value to_json()
		{
			nlohmann::json json = *this;
			Json::Value data;
			Json::Reader reader;

			reader.parse(json.dump(), data);

			return data;
		}

		NLOHMANN_DEFINE_TYPE_INTRUSIVE(OrderResponse, id, name, customerId, totalPrice, discount, grandTotal, totalItem, status, expired, detail, createdAt, updatedAt)
	};
}