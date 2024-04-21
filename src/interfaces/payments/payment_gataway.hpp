#pragma once

#include <pch.h>
#include <payment/models/Payments.h>

using namespace drogon;
using namespace orm;
using namespace drogon_model::gaboot;


namespace gaboot
{
	struct payment_status
	{
		std::string id;
		int status_code;
		std::string status_message;

		NLOHMANN_DEFINE_TYPE_INTRUSIVE(payment_status, id, status_code, status_message)
	};

	struct bank_response
	{
		std::string bank;
		std::string va_number;

		NLOHMANN_DEFINE_TYPE_INTRUSIVE(bank_response, bank, va_number)
	};

	struct payment_gateway : public payment_status
	{
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
		payment_status m_status;

		bool empty()
		{
			return id.empty() && status_code == 0 && status_message.empty() &&
				order_id.empty() && gross_amount.empty() && currency.empty() &&
				expiry_time.empty() && merchant_id.empty() && payment_type.empty() &&
				transaction_id.empty() && transaction_status.empty() &&
				transaction_time.empty() && fraud_status.empty() && va_numbers.empty();
		}

		void from_json(nlohmann::json const& json)
		{
			if (json["status_code"] >= 200 && json["status_code"] <= 299)
			{
				*this = json.get<payment_gateway>();
			}
			else
			{
				m_status = json.get<payment_status>();
			}
		}
		Json::Value to_json()
		{
			nlohmann::json success = *this;
			nlohmann::json fail = m_status;

			Json::Value data;
			Json::Reader reader;

			reader.parse((this->status_code >= 200 && this->status_code <= 299) ? success.dump() : fail.dump(), data);

			return data;
		}

		template<typename U>
		std::enable_if<std::is_same<U, std::string>::value, void>::type operator=(U const& payment)
		{
			if (util::is_json(payment))
			{
				auto j = nlohmann::json::parse(payment);

				this->from_json(j);
			}
		}

		template<typename U>
		std::enable_if<std::is_same<U, Payments>::value, void>::type operator=(U const& payment)
		{
			this->id = payment.getValueOfId();
			this->order_id = payment.getValueOfOrderId();
			this->gross_amount = payment.getValueOfGrossAmount();
			this->transaction_id = payment.getValueOfTransactionId();
			this->transaction_status = payment.getValueOfTransactionStatus();
			this->transaction_time = payment.getValueOfTransactionTime();
			this->va_numbers[0].va_number = payment.getValueOfVaNumber();
			this->currency = payment.getValueOfCurrency();
			this->expiry_time = payment.getValueOfExpiryTime();
			this->fraud_status = payment.getValueOfFraudStatus();
			this->merchant_id = payment.getValueOfMerchantId();
			this->payment_type = payment.getValueOfPaymentType();
			this->va_numbers[0].bank = payment.getValueOfBank();
		}

		NLOHMANN_DEFINE_TYPE_INTRUSIVE(payment_gateway, status_code, status_message, order_id, gross_amount, currency, expiry_time, merchant_id, payment_type, transaction_id, transaction_status, transaction_time, fraud_status, va_numbers)
	}; 
}