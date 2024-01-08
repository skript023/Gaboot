#pragma once

#include <customer_detail.hpp>
#include <bank_transfer.hpp>
#include <order.hpp>

namespace gaboot
{
	struct transaction
	{
		std::string m_payment_type;
		order m_transaction_details;
		customer_detail m_customer_detail;
		bank_transfer m_bank_transfer;

		void from_json(Json::Value const& json)
		{
			this->m_payment_type = json["payment_type"].asString();
			this->m_transaction_details.order_id = json["transaction_details"]["order_id"].asString();
			this->m_transaction_details.gross_amount = json["transaction_details"]["gross_amount"].asInt();
			this->m_bank_transfer.m_bank = json["bank_transfer"]["bank"].asString();

			this->m_customer_detail.m_first_name = json["customer"]["first_name"].asString();
            this->m_customer_detail.m_last_name = json["customer"]["last_name"].asString();
            this->m_customer_detail.m_email = json["customer"]["email"].asString();
            this->m_customer_detail.m_phone = json["customer"]["phone"].asString();
            this->m_customer_detail.m_billing_address.m_first_name = json["customer"]["billing"]["first_name"].asString();
            this->m_customer_detail.m_billing_address.m_last_name = json["customer"]["billing"]["last_name"].asString();
            this->m_customer_detail.m_billing_address.m_email = json["customer"]["billing"]["email"].asString();
            this->m_customer_detail.m_billing_address.m_phone = json["customer"]["billing"]["phone"].asString();
            this->m_customer_detail.m_billing_address.m_address = json["customer"]["billing"]["address"].asString();
            this->m_customer_detail.m_billing_address.m_city = json["customer"]["billing"]["city"].asString();
            this->m_customer_detail.m_billing_address.m_postal_code = json["customer"]["billing"]["postal_code"].asString();
            this->m_customer_detail.m_billing_address.m_country_code = json["customer"]["billing"]["country_code"].asString();
		}

		nlohmann::json to_json()
		{
			nlohmann::json json;
			json["payment_type"] = m_payment_type;
			json["transaction_details"]["order_id"] = m_transaction_details.order_id;
			json["transaction_details"]["gross_amount"] = m_transaction_details.order_id;
			json["bank_transfer"]["bank"] = m_bank_transfer.m_bank;
			
            json["customer"]["first_name"] = m_customer_detail.m_first_name;
            json["customer"]["last_name"] = m_customer_detail.m_last_name;
            json["customer"]["email"] = m_customer_detail.m_email;
            json["customer"]["phone"] = m_customer_detail.m_phone;
            json["customer"]["billing_address"]["first_name"] = m_customer_detail.m_billing_address.m_first_name;
            json["customer"]["billing_address"]["last_name"] = m_customer_detail.m_billing_address.m_last_name;
            json["customer"]["billing_address"]["email"] = m_customer_detail.m_billing_address.m_email;
            json["customer"]["billing_address"]["phone"] = m_customer_detail.m_billing_address.m_phone;
            json["customer"]["billing_address"]["address"] = m_customer_detail.m_billing_address.m_address;
            json["customer"]["billing_address"]["city"] = m_customer_detail.m_billing_address.m_city;
            json["customer"]["billing_address"]["postal_code"] = m_customer_detail.m_billing_address.m_postal_code;
            json["customer"]["billing_address"]["country_code"] = m_customer_detail.m_billing_address.m_country_code;

            return json;
		}
	};
}