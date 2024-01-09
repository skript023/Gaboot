#pragma once

#include <pch.h>

namespace gaboot
{
	struct bank_response
	{
		std::string m_bank;
		std::string m_va_number;
	};

	struct midtrans_response
	{
		int m_status_code;
		std::string m_status_message;
		std::string m_currency;
		std::string m_expiry_time;
		std::string m_merchant_id;
		std::string m_payment_type;
		std::string m_transaction_id;
		std::string m_transaction_status;
		std::string m_transaction_time;
		std::string m_fraud_status;
		std::string m_id;
		std::vector<bank_response> m_va_numbers;
		bool m_is_success = false;

		void from_json(nlohmann::json const& json)
		{
			if (!json.contains("id"))
			{
				m_currency = json.contains("currency") ? json["currency"] : "";
				m_expiry_time = json.contains("expiry_time") ? json["expiry_time"] : "";
				m_merchant_id = json.contains("merchant_id") ? json["merchant_id"] : "";
				m_payment_type = json.contains("payment_type") ? json["payment_type"] : "";
				m_transaction_id = json.contains("transaction_id") ? json["transaction_id"] : "";
				m_transaction_status = json.contains("transaction_status") ? json["transaction_status"] : "";
				m_transaction_time = json.contains("transaction_time") ? json["transaction_time"] : "";
				m_fraud_status = json.contains("fraud_status") ? json["fraud_status"] : "";

				if (json.contains("va_numbers"))
				{
					for (auto& va_number : json["va_numbers"])
					{
						bank_response virtual_account;

						virtual_account.m_bank = va_number["bank"];
						virtual_account.m_va_number = va_number["va_number"];

						m_va_numbers.emplace_back(virtual_account);
					}
				}

				m_is_success = true;
			}

			m_status_code = json.contains("status_code") ? json["status_code"] : "";
			m_status_message = json.contains("status_message") ? json["status_message"] : "";
			m_id = json.contains("id") ? json["id"] : "";

			LOG(INFO) << json.dump();
		}

		Json::Value to_json()
		{
			Json::Value json;
			Json::Value va_nums(Json::arrayValue);

			if (m_is_success)
			{
				json["currency"] = this->m_currency;
				json["expiry_time"] = this->m_expiry_time;
				json["merchant_id"] = this->m_merchant_id;
				json["payment_type"] = this->m_payment_type;
				json["transaction_id"] = this->m_transaction_id;
				json["transaction_status"] = this->m_transaction_status;
				json["transaction_time"] = this->m_transaction_time;
				json["fraud_status"] = this->m_fraud_status;

				for (auto& account : m_va_numbers)
				{
					Json::Value jsonObject;
					jsonObject["bank"] = account.m_bank;
					jsonObject["va_number"] = account.m_va_number;
					va_nums.append(jsonObject);
				}

				json["va_numbers"] = va_nums;
			}

			json["status_code"] = this->m_status_code;
			json["status_message"] = this->m_status_message;
			
			if (!m_is_success)
				json["id"] = this->m_id;

			LOG(INFO) << json.toStyledString();

			return json;
		}
	}; 
}