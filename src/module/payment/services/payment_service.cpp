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

		payment_gateway payment;

		try
		{
			TRANSACTION_BEGIN_CLAUSE(json, &payment)
			{
				Payments payments;
				payments.setBank(payment.m_va_numbers[0].m_bank);
				payments.setVaNumber(payment.m_va_numbers[0].m_va_number);
				payments.setCurrency(payment.m_currency);
				payments.setExpiryTime(payment.m_expiry_time);
				payments.setMerchantId(payment.m_merchant_id);
				payments.setPaymentType(payment.m_payment_type);
				payments.setTransactionId(payment.m_transaction_id);
				payments.setTransactionStatus(payment.m_transaction_status);
				payments.setTransactionTime(payment.m_transaction_time);
				payments.setFraudStatus(payment.m_fraud_status);
				payments.setName(json["name"].asString());
				payments.setDescription(json["description"].asString());

				payments.setCreatedAt(trantor::Date::now());
				payments.setUpdatedAt(trantor::Date::now());

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
    HttpResponsePtr payment_service::findOne(HttpRequestPtr const& req, std::string &&transactionId)
    {
        TRY_CLAUSE
		{
			if (transactionId.empty() || util::is_numeric(transactionId))
			{
				throw BadRequestException("Parameter is invalid");
			}

			auto args = Criteria(Payments::Cols::_transaction_id, CompareOperator::EQ, transactionId);
			auto transaction = db().findBy(args);

			if (transaction.empty())
			{
				throw NotFoundException(fmt::format("Unable found transaction id {}", transactionId));
			}

			m_response.m_message = "Transaction found";
			m_response.m_success = true;
			m_response.m_data = transaction[0].toJson();

			return HttpResponse::newHttpJsonResponse(m_response.to_json());
		} EXCEPT_CLAUSE
    }
    HttpResponsePtr payment_service::callback(HttpRequestPtr const &req)
    {
		TRY_CLAUSE
		{
			auto& json = *req->getJsonObject();

			json["status_code"] = stoi(json["status_code"].asString());

			payment_gateway payment;

			payment.from_json(util::to_nlohmann_json(json));

			auto args = Criteria(Payments::Cols::_transaction_id, CompareOperator::EQ, payment.m_transaction_id);

			switch (jenkins::hash(payment.m_transaction_status))
			{
				case JENKINS_HASH("settlement"):
					if (auto record = db().updateBy({ Payments::Cols::_transaction_status }, args, payment.m_transaction_status); !record)
						throw InternalServerErrorException("Failed update transaction");

					m_response.m_message = "Payment status updated as paid";
					m_response.m_success = true;

					return HttpResponse::newHttpJsonResponse(m_response.to_json());
				case JENKINS_HASH("expired"):
					if (auto record = db().updateBy({ Payments::Cols::_transaction_status }, args, payment.m_transaction_status); !record)
						throw InternalServerErrorException("Failed update transaction");

					m_response.m_message = "Payment status updated as expired";
					m_response.m_success = true;

					return HttpResponse::newHttpJsonResponse(m_response.to_json());
			}

			m_response.m_message = "Payment callback called successfully, but nothing to be updated";
			m_response.m_success = true;

			return HttpResponse::newHttpJsonResponse(m_response.to_json());
		} EXCEPT_CLAUSE
	}
}