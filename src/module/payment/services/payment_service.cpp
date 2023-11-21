#include "payment_service.hpp"
#include "payment_processing.hpp"

namespace gaboot
{
	HttpResponsePtr gaboot::payment_service::create(HttpRequestPtr const& req)
	{
		auto& json = *req->getJsonObject();

		nlohmann::ordered_json midtrans;

		try
		{
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
			auto response = HttpResponse::newHttpResponse();
			response->setStatusCode(k500InternalServerError);
			response->setBody(e.what());

			return response;
		}
	}
}