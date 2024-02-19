#include "payment_service.hpp"
#include "payment_processing.hpp"

#include "exception/exception.hpp"
#include "formatter/money_formatter.hpp"

#include "payments/item_detail.hpp"
#include "payments/customer_detail.hpp"
#include "payments/payment_gataway.hpp"

#include "util/hash/jenkins.hpp"

namespace gaboot
{
	HttpResponsePtr gaboot::payment_service::create(HttpRequestPtr const& req)
	{
		auto& json = *req->getJsonObject();

		payment_gataway payment;

		try
		{
			TRANSACTION_BEGIN_CLAUSE(json, &payment)
			{
				Payments payments;
				payments.setBank(payment.m_va_numbers[0].m_bank);
				payments.setVanumber(payment.m_va_numbers[0].m_va_number);
				payments.setCurrency(payment.m_currency);
				payments.setExpiryTime(payment.m_expiry_time);
				payments.setMerchantid(payment.m_merchant_id);
				payments.setPaymenttype(payment.m_payment_type);
				payments.setTransactionid(payment.m_transaction_id);
				payments.setTransactionstatus(payment.m_transaction_status);
				payments.setTransactiontime(payment.m_transaction_time);
				payments.setFraudstatus(payment.m_fraud_status);
				payments.setName(json["name"].asString());
				payments.setDescription(json["description"].asString());

				payments.setCreatedat(trantor::Date::now());
				payments.setUpdatedat(trantor::Date::now());

				db().insert(payments);

				TRANSACTION_SUCCESS(payment);

			} TRANSACTION_END_CLAUSE

			TRANSACTION_FAILED(payment);
		}
		catch (const std::exception& e)
		{
			TRANSACTION_ERROR(e.what());
		}
	}
	HttpResponsePtr payment_service::callback(HttpRequestPtr const& req)
	{
		TRY_CLAUSE
		{
			auto& json = *req->getJsonObject();

			std::string transactionStatus = json["transaction_status"].asString();
			std::string transactionId = json["transaction_id"].asString();

			switch (jenkins::hash(transactionStatus))
			{
			case JENKINS_HASH("settlement"):
			{
				auto args = Criteria(Payments::Cols::_transactionId, CompareOperator::EQ, transactionId);

				if (auto record = db().updateBy({ Payments::Cols::_transactionStatus }, args, transactionStatus); !record)
					throw CustomException<k500InternalServerError>("Failed update transaction");

				m_response.m_message = "Payment status updated as paid";
				m_response.m_success = true;
				m_response.m_data = m_data;

				return HttpResponse::newHttpJsonResponse(m_response.to_json());
			}
			case JENKINS_HASH("expired"):
			{
				auto args = Criteria(Payments::Cols::_transactionId, CompareOperator::EQ, transactionId);

				if (auto record = db().updateBy({ Payments::Cols::_transactionStatus }, args, transactionStatus); !record)
					throw CustomException<k500InternalServerError>("Failed update transaction");

				m_response.m_message = "Payment status updated as expired";
				m_response.m_success = true;
				m_response.m_data = m_data;

				return HttpResponse::newHttpJsonResponse(m_response.to_json());
			}
			default:
				break;
			}
		} EXCEPT_CLAUSE
	}
}