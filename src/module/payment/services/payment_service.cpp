#include "payment_service.hpp"
#include "payment_processing.hpp"

#include "util/exception.hpp"
#include "util/money_formatter.hpp"

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
			m_items.from_json(json);
			m_customer.from_json(json);

			g_payment_processing->item_details(m_items.to_json());
			g_payment_processing->customer_details(&m_customer);
			g_payment_processing->bank_transfer(json["order_id"].asString(), json["bank_type"].asString(), json["gross_amount"].asInt());

			if (g_payment_processing->make_payment(midtrans))
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
			}

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