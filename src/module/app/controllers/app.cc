#include "app.h"

// Add definition of your processing function here
namespace gaboot
{
	void app::main(HttpRequestPtr const& req, response_t&& callback)
	{
        auto response = HttpResponse::newHttpViewResponse("login");

		return callback(response);
	}
	void app::test(HttpRequestPtr const& request, response_t&& callback)
	{
        std::string token = fmt::format("Basic {}", SERVER_KEY);

        cpr::Body body = R"({
          "payment_type": "bank_transfer",
          "transaction_details": {
              "order_id": "order-101",
              "gross_amount": 44000
          },
          "bank_transfer":{
              "bank": "bca"
          }
        })"_json.dump();

        cpr::Url url = "https://api.sandbox.midtrans.com/v2/charge";
        cpr::Header header = {
            {"Accept", "application/json"},
            { "Content-Type", "application/json" },
            { "Authorization", token}
        };

        auto result = cpr::PostAsync(url, header, body).get();

        auto json = nlohmann::json::parse(result.text);

        auto response = HttpResponse::newHttpResponse();
        response->setStatusCode((HttpStatusCode)std::stoi(json["status_code"].get<std::string>()));
        response->setBody(result.text);
        response->setContentTypeCode(CT_APPLICATION_JSON);

        callback(response);
	}
}