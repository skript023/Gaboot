#include "payment_service.hpp"
#include "payment_processing.hpp"

#include "exception/exception.hpp"
#include "formatter/money_formatter.hpp"

#include "util/hash/jenkins.hpp"

namespace gaboot
{
	payment_service::payment_service():
		m_database(std::make_unique<Mapper<Payments>>(DATABASE_CLIENT)),
		m_response(std::make_unique<response_data<payment_gateway>>())
	{
		LOG(INFO) << "Payment service registered";
	}
	payment_service::~payment_service() noexcept
	{
		m_database.reset();
		m_response.reset();
	}
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

				m_database->insert(payments);

				m_response->m_success = true;
				m_response->m_message = "Payment success created";
				m_response->m_data = *payment;

				TRANSACTION_SUCCESS(m_response);

			} TRANSACTION_END_CLAUSE

			m_response->m_success = false;
			m_response->m_message = "Failed create payment";
			m_response->m_data = *payment;

			TRANSACTION_FAILED(m_response);
		}
		catch (const std::exception& e)
		{
			TRANSACTION_ERROR(e.what());
		}
	}
    HttpResponsePtr payment_service::findOne(HttpRequestPtr const& req, std::string &&transactionId)
    {
		if (transactionId.empty())
		{
			throw BadRequestException("Parameter is invalid");
		}

		auto args = Criteria(Payments::Cols::_transaction_id, CompareOperator::EQ, std::move(transactionId));
		const auto transaction = m_database->findOne(args);

		if (transaction.toJson().empty())
		{
			throw NotFoundException(fmt::format("Unable found transaction id {}", std::move(transactionId)));
		}

		m_response->m_message = "Transaction found";
		m_response->m_success = true;
		m_response->m_data = transaction;

		return m_response->json();
    }
    HttpResponsePtr payment_service::callback(HttpRequestPtr const &req)
    {
		auto& json = *req->getJsonObject();

		json["status_code"] = stoi(json["status_code"].asString());

		const auto payment = std::make_unique<payment_gateway>();

		*payment = json.toStyledString();

		auto args = Criteria(Payments::Cols::_transaction_id, CompareOperator::EQ, payment->transaction_id);

		switch (fnv1a_32(payment->transaction_status))
		{
		case "settlement"_hash:
			if (auto record = m_database->updateBy({ Payments::Cols::_transaction_status }, args, payment->transaction_status); !record)
				throw NotFoundException("Failed update transaction, transaction doesn't exist");

			m_response->m_message = "Payment status updated as paid";
			m_response->m_success = true;

			return m_response->json();
		case "capture"_hash:
			if (auto record = m_database->updateBy({ Payments::Cols::_transaction_status }, args, payment->transaction_status); !record)
				throw NotFoundException("Failed update transaction, transaction doesn't exist");

			m_response->m_message = "Payment status updated as capture";
			m_response->m_success = true;

			return m_response->json();
		case "cancel"_hash:
			if (auto record = m_database->updateBy({ Payments::Cols::_transaction_status }, args, payment->transaction_status); !record)
				throw NotFoundException("Failed update transaction, transaction doesn't exist");

			m_response->m_message = "Payment status updated as cancel";
			m_response->m_success = true;

			return m_response->json();
		case "deny"_hash:
			if (auto record = m_database->updateBy({ Payments::Cols::_transaction_status }, args, payment->transaction_status); !record)
				throw NotFoundException("Failed update transaction, transaction doesn't exist");

			m_response->m_message = "Payment status updated as deny, because fraud detected";
			m_response->m_success = true;

			return m_response->json();
		case "failure"_hash:
			if (auto record = m_database->updateBy({ Payments::Cols::_transaction_status }, args, payment->transaction_status); !record)
				throw NotFoundException("Failed update transaction, transaction doesn't exist");

			m_response->m_message = "Payment status updated as failure";
			m_response->m_success = true;

			return m_response->json();
		case "expired"_hash:
			if (auto record = m_database->updateBy({ Payments::Cols::_transaction_status }, args, payment->transaction_status); !record)
				throw NotFoundException("Failed update transaction, transaction doesn't exist");

			m_response->m_message = "Payment status updated as expired";
			m_response->m_success = true;

			return m_response->json();
		}

		m_response->m_message = "Payment callback called successfully, but nothing to be updated";
		m_response->m_success = true;
		m_response->m_data = *payment;

		return m_response->json();
	}
}