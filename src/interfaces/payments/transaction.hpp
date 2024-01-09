#pragma once

#include <order.hpp>
#include <item_detail.hpp>
#include <bank_transfer.hpp>
#include <customer_detail.hpp>

namespace gaboot
{
	struct transaction
	{
		std::string m_payment_type;
		order m_transaction_details;
		customer_detail m_customer_detail;
		bank_transfer m_bank_transfer;
		std::vector<item_detail> m_item_details;
		bool m_is_customer_detail = false;
		bool m_is_item_detail = false;
		bool m_is_billing_address = false;

		void from_json(Json::Value const& json)
		{
			this->m_payment_type = "bank_transfer";
			this->m_transaction_details.order_id = json["order_id"].asString();
			this->m_transaction_details.gross_amount = json["gross_amount"].asInt();
			this->m_bank_transfer.m_bank = json["bank_type"].asString();

			if (json.isMember("product"))
			{
				for (auto& product : json["product"])
				{
					item_detail items;

					items.m_id = product["id"].asInt64();
					items.m_name = product["name"].asString();
					items.m_price = product["price"].asDouble();
					items.m_quantity = product["quantity"].asInt64();

					m_item_details.emplace_back(items);
				}

				m_is_item_detail = true;
			}

			if (json.isMember("customer"))
			{
				this->m_customer_detail.m_first_name = json["customer"]["first_name"].asString();
				this->m_customer_detail.m_last_name = json["customer"]["last_name"].asString();
				this->m_customer_detail.m_email = json["customer"]["email"].asString();
				this->m_customer_detail.m_phone = json["customer"]["phone"].asString();
				
				if (json.isMember("billing"))
				{
					this->m_customer_detail.m_billing_address.m_first_name = json["customer"]["billing"]["first_name"].asString();
					this->m_customer_detail.m_billing_address.m_last_name = json["customer"]["billing"]["last_name"].asString();
					this->m_customer_detail.m_billing_address.m_email = json["customer"]["billing"]["email"].asString();
					this->m_customer_detail.m_billing_address.m_phone = json["customer"]["billing"]["phone"].asString();
					this->m_customer_detail.m_billing_address.m_address = json["customer"]["billing"]["address"].asString();
					this->m_customer_detail.m_billing_address.m_city = json["customer"]["billing"]["city"].asString();
					this->m_customer_detail.m_billing_address.m_postal_code = json["customer"]["billing"]["postal_code"].asString();
					this->m_customer_detail.m_billing_address.m_country_code = json["customer"]["billing"]["country_code"].asString();

					m_is_billing_address = true;
				}

				m_is_customer_detail = true;
			}
		}

		nlohmann::json to_json()
		{
			nlohmann::json json;
			json["payment_type"] = m_payment_type;
			json["transaction_details"]["order_id"] = m_transaction_details.order_id;
			json["transaction_details"]["gross_amount"] = m_transaction_details.gross_amount;
			json["bank_transfer"]["bank"] = m_bank_transfer.m_bank;

			if (m_is_item_detail)
			{
				auto items = nlohmann::json::array();

				for (auto& item : m_item_details)
				{
					items.push_back({
						{"id", item.m_id},
						{"name", item.m_name},
						{"price", item.m_price},
						{"quantity", item.m_quantity}
					});
				}

				json["item_details"] = items;
			}

			if (m_is_customer_detail)
			{
				json["customer_details"]["first_name"] = m_customer_detail.m_first_name;
				json["customer_details"]["last_name"] = m_customer_detail.m_last_name;
				json["customer_details"]["email"] = m_customer_detail.m_email;
				json["customer_details"]["phone"] = m_customer_detail.m_phone;
				
				if (m_is_billing_address)
				{
					json["customer_details"]["billing_address"]["first_name"] = m_customer_detail.m_billing_address.m_first_name;
					json["customer_details"]["billing_address"]["last_name"] = m_customer_detail.m_billing_address.m_last_name;
					json["customer_details"]["billing_address"]["email"] = m_customer_detail.m_billing_address.m_email;
					json["customer_details"]["billing_address"]["phone"] = m_customer_detail.m_billing_address.m_phone;
					json["customer_details"]["billing_address"]["address"] = m_customer_detail.m_billing_address.m_address;
					json["customer_details"]["billing_address"]["city"] = m_customer_detail.m_billing_address.m_city;
					json["customer_details"]["billing_address"]["postal_code"] = m_customer_detail.m_billing_address.m_postal_code;
					json["customer_details"]["billing_address"]["country_code"] = m_customer_detail.m_billing_address.m_country_code;
				}
			}

            return json;
		}
	};
}