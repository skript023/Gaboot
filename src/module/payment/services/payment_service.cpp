#include "payment_service.hpp"
#include "payment_processing.hpp"

#include "exception/exception.hpp"
#include "formatter/money_formatter.hpp"

#include "interfaces/item_detail.hpp"
#include "interfaces/customer_detail.hpp"

namespace gaboot
{
	HttpResponsePtr gaboot::payment_service::create(HttpRequestPtr const& req)
	{
		auto& json = *req->getJsonObject();

		nlohmann::ordered_json midtrans;

		try
		{
			TRANSACTION_BEGIN_CLAUSE(json, midtrans)
			{
				Payments payments;
				payments.setBank(midtrans["va_numbers"][0]["bank"]);
				payments.setVanumber(midtrans["va_numbers"][0]["va_number"]);
				payments.setCurrency(midtrans["currency"]);
				payments.setExpiryTime(midtrans["expiry_time"]);
				payments.setMerchantid(midtrans["merchant_id"]);
				payments.setPaymenttype(midtrans["payment_type"]);
				payments.setTransactionid(midtrans["transaction_id"]);
				payments.setTransactionstatus(midtrans["transaction_status"]);
				payments.setTransactiontime(midtrans["transaction_time"]);
				payments.setFraudstatus(midtrans["fraud_status"]);
				payments.setName(json["name"].asString());
				payments.setDescription(json["description"].asString());

				payments.setCreatedat(trantor::Date::now());
				payments.setUpdatedat(trantor::Date::now());

				db().insert(payments);

				auto response = HttpResponse::newHttpResponse();
				response->setContentTypeCode(CT_APPLICATION_JSON);
				response->setBody(midtrans.dump());
				response->setStatusCode(k201Created);

				return response;
			} TRANSACTION_END_CLAUSE
			
			auto response = HttpResponse::newHttpResponse();
			response->setStatusCode((HttpStatusCode)std::stoi(midtrans["status_code"].get<std::string>()));
			response->setContentTypeCode(CT_APPLICATION_JSON);
			response->setBody(midtrans.dump());

			return response;
		}
		catch (const std::exception& e)
		{
			return CustomException<k500InternalServerError>(fmt::format("Unable to create payment, error caught on {}", e.what())).response();
		}
	}
	HttpResponsePtr payment_service::notification(HttpRequestPtr const& req)
	{
		try
		{
			auto& json = *req->getJsonObject();

			std::string transactionStatus = json["transaction_status"].asString();

			auto args = Criteria(Payments::Cols::_transactionId, CompareOperator::EQ, json["transaction_id"].asString());

			db().updateBy({ Payments::Cols::_transactionStatus }, args, transactionStatus);

			m_response.m_message = "Payment status updated successfully";
			m_response.m_success = true;
			m_response.m_data = m_data;

			return HttpResponse::newHttpJsonResponse(m_response.to_json());
		}
		catch (const std::exception& e)
		{
			return CustomException<k500InternalServerError>(fmt::format("Unable to update payment, error caught on {}", e.what())).response();
		}
	}
}