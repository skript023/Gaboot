#include "payment_service.hpp"
#include "payment_processing.hpp"

#include "exception/exception.hpp"
#include "formatter/money_formatter.hpp"

#include "util/hash/jenkins.hpp"

namespace gaboot
{
	HttpResponsePtr gaboot::payment_service::create(HttpRequestPtr const& req)
	{
		auto& json = *req->getJsonObject();

		const auto payment = std::make_unique<payment_gateway>();

		try
		{
			TRANSACTION_BEGIN_CLAUSE(json, &(*payment))
			{
				Payments payments;
				payments.setOrderId(payment->order_id);
				payments.setGrossAmount(stod(payment->gross_amount));
				payments.setBank(payment->va_numbers[0].bank);
				payments.setVaNumber(payment->va_numbers[0].va_number);
				payments.setCurrency(payment->currency);
				payments.setExpiryTime(payment->expiry_time);
				payments.setMerchantId(payment->merchant_id);
				payments.setPaymentType(payment->payment_type);
				payments.setTransactionId(payment->transaction_id);
				payments.setTransactionStatus(payment->transaction_status);
				payments.setTransactionTime(payment->transaction_time);
				payments.setFraudStatus(payment->fraud_status);
				payments.setName(json["name"].asString());
				payments.setDescription(json["description"].asString());

				payments.setCreatedAt(trantor::Date::now());
				payments.setUpdatedAt(trantor::Date::now());

				db().insert(payments);

				m_response.m_success = true;
				m_response.m_message = "Payment success created";
				m_response.m_data = *payment;

				TRANSACTION_SUCCESS(m_response);

			} TRANSACTION_END_CLAUSE

			m_response.m_success = false;
			m_response.m_message = "Failed create payment";
			m_response.m_data = *payment;

			TRANSACTION_FAILED(m_response);
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
			if (transactionId.empty())
			{
				throw BadRequestException("Parameter is invalid");
			}

			auto args = Criteria(Payments::Cols::_transaction_id, CompareOperator::EQ, std::move(transactionId));
			const auto transaction = db().findOne(args);

			if (transaction.toJson().empty())
			{
				throw NotFoundException(fmt::format("Unable found transaction id {}", transactionId));
			}

			m_response.m_message = "Transaction found";
			m_response.m_success = true;
			m_response.m_data = transaction;

			return HttpResponse::newHttpJsonResponse(m_response.to_json());
		} EXCEPT_CLAUSE
    }
    HttpResponsePtr payment_service::callback(HttpRequestPtr const &req)
    {
		TRY_CLAUSE
		{
			auto& json = *req->getJsonObject();

			json["status_code"] = stoi(json["status_code"].asString());

			const auto payment = std::make_unique<payment_gateway>();

			*payment = json.toStyledString();

			auto args = Criteria(Payments::Cols::_transaction_id, CompareOperator::EQ, payment->transaction_id);

			switch (jenkins::hash(payment->transaction_status))
			{
				case JENKINS_HASH("settlement"):
					if (auto record = db().updateBy({ Payments::Cols::_transaction_status }, args, payment->transaction_status); !record)
						throw InternalServerErrorException("Failed update transaction, transaction doesn't exist");

					m_response.m_message = "Payment status updated as paid";
					m_response.m_success = true;

					return HttpResponse::newHttpJsonResponse(m_response.to_json());
				case JENKINS_HASH("capture"):
					if (auto record = db().updateBy({ Payments::Cols::_transaction_status }, args, payment->transaction_status); !record)
						throw InternalServerErrorException("Failed update transaction, transaction doesn't exist");

					m_response.m_message = "Payment status updated as capture";
					m_response.m_success = true;

					return HttpResponse::newHttpJsonResponse(m_response.to_json());
				case JENKINS_HASH("cancel"):
					if (auto record = db().updateBy({ Payments::Cols::_transaction_status }, args, payment->transaction_status); !record)
						throw InternalServerErrorException("Failed update transaction, transaction doesn't exist");

					m_response.m_message = "Payment status updated as cancel";
					m_response.m_success = true;

					return HttpResponse::newHttpJsonResponse(m_response.to_json());
				case JENKINS_HASH("deny"):
					if (auto record = db().updateBy({ Payments::Cols::_transaction_status }, args, payment->transaction_status); !record)
						throw InternalServerErrorException("Failed update transaction, transaction doesn't exist");

					m_response.m_message = "Payment status updated as deny, because fraud detected";
					m_response.m_success = true;

					return HttpResponse::newHttpJsonResponse(m_response.to_json());
				case JENKINS_HASH("failure"):
					if (auto record = db().updateBy({ Payments::Cols::_transaction_status }, args, payment->transaction_status); !record)
						throw InternalServerErrorException("Failed update transaction, transaction doesn't exist");

					m_response.m_message = "Payment status updated as failure";
					m_response.m_success = true;

					return HttpResponse::newHttpJsonResponse(m_response.to_json());
				case JENKINS_HASH("expired"):
					if (auto record = db().updateBy({ Payments::Cols::_transaction_status }, args, payment->transaction_status); !record)
						throw InternalServerErrorException("Failed update transaction, transaction doesn't exist");

					m_response.m_message = "Payment status updated as expired";
					m_response.m_success = true;

					return HttpResponse::newHttpJsonResponse(m_response.to_json());
			}

			m_response.m_message = "Payment callback called successfully, but nothing to be updated";
			m_response.m_success = true;
			m_response.m_data = *payment;

			return HttpResponse::newHttpJsonResponse(m_response.to_json());
		} EXCEPT_CLAUSE
	}
}