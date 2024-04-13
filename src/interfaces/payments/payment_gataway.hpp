#pragma once

#include <pch.h>

namespace gaboot
{
	struct payment_failed
	{
		std::string id;
		int status_code;
		std::string status_message;

		NLOHMANN_DEFINE_TYPE_INTRUSIVE(payment_failed, id, status_code, status_message)
	};

	struct bank_response
	{
		std::string bank;
		std::string va_number;

		NLOHMANN_DEFINE_TYPE_INTRUSIVE(bank_response, bank, va_number)
	};

	struct payment_gateway : public payment_failed
	{
		payment_gateway() = default;
		payment_gateway(nlohmann::json const& json)
		{
			if (json["status_code"] == 201)
			{
				*this = json.get<payment_gateway>();
			}
			else
			{
				m_failed = json.get<payment_failed>();
			}
		}

		std::string order_id;
		std::string gross_amount;
		std::string currency;
		std::string expiry_time;
		std::string merchant_id;
		std::string payment_type;
		std::string transaction_id;
		std::string transaction_status;
		std::string transaction_time;
		std::string fraud_status;
		std::vector<bank_response> va_numbers;
		payment_failed m_failed;

		NLOHMANN_DEFINE_TYPE_INTRUSIVE(payment_gateway, status_code, status_message, order_id, gross_amount, currency, expiry_time, merchant_id, payment_type, transaction_id, transaction_status, transaction_time, fraud_status, va_numbers)

		void from_json(nlohmann::json const& json)
		{
			if (json["status_code"] == 201)
			{
				*this = json.get<payment_gateway>();
			}
			else
			{
				m_failed = json.get<payment_failed>();
			}
		}
		Json::Value to_json()
		{
			nlohmann::json json;
			if (this->status_code != 201)
				json = m_failed;
			else
				json = *this;

			Json::Value data;
			Json::Reader reader;

			reader.parse(json.dump(), data);

			return data;
		}
	}; 
}