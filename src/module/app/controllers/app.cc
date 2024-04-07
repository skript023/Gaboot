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

        std::string body = R"({
          "payment_type": "bank_transfer",
          "transaction_details": {
              "order_id": "order-101",
              "gross_amount": 44000
          },
          "bank_transfer":{
              "bank": "bca"
          }
        })"_json.dump();

        std::string url = "https://api.sandbox.midtrans.com";
        cpr::Header header = {
            {"Accept", "application/json"},
            { "Content-Type", "application/json" },
            { "Authorization", token}
        };

        auto client = HttpClient::newHttpClient(url);
        auto req = HttpRequest::newHttpRequest();
        req->setMethod(Post);
        req->addHeader("Accept", "application/json");
        req->addHeader("Content-Type", "application/json");
        req->addHeader("Authorization", token);
        req->setBody(body);
        req->setPath("/v2/charge");
        client->sendRequest(req, [callback = std::move(callback)](ReqResult result, const HttpResponsePtr& res) {
            if (result == ReqResult::Ok)
            {
                auto response = HttpResponse::newHttpResponse();
                response->setContentTypeCode(res->getContentType());
                response->setBody(res->getBody().data());
                response->setStatusCode(res->getStatusCode());

                callback(response);
            }
        });
	}
}