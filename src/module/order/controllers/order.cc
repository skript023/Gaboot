#include "order.h"

// Add definition of your processing function here
namespace gaboot
{
    void order::create(HttpRequestPtr const& req, response_t&& callback)
    {
        try
        {
            std::string order_id = "YOUR-ORDERID-123456";
            std::string first_name = "budi";
            std::string last_name = "pratama";
            std::string email = "budi.pra@example.com";
            std::string phone = "08111222333";
            std::string token = fmt::format("Basic {}", SERVER_KEY);

            nlohmann::ordered_json request_payment = {
                {
                    "transaction_details", {
                        {"order_id", order_id},
                        {"gross_amount", 12000000}
                    },
                },
                {
                    "credit_card", {
                        {"secure", true}
                    },
                },
                {
                    "customer_details", {
                        {"first_name", first_name},
                        {"last_name", last_name},
                        {"email", email},
                        {"phone", phone}
                    }
                }
            };

            cpr::Url url = "https://app.sandbox.midtrans.com/snap/v1/transactions";
            cpr::Body body = request_payment.dump();
            cpr::Header header = {
                {"Accept", "application/json"},
                { "Content-Type", "application/json" },
                { "Authorization", token}
            };

            auto async = cpr::PostAsync(url, body, header);
            auto res = async.get();

            auto midtrans = nlohmann::ordered_json::parse(res.text);

            if (res.status_code == 201)
            {
                midtrans["message"] = "Create transaction success";
                midtrans["success"] = true;

                auto response = HttpResponse::newHttpResponse();
                response->setBody(midtrans.dump());
                response->setContentTypeCode(CT_APPLICATION_JSON);

                return callback(response);
            }
            else
            {
                midtrans["message"] = "Create transaction failed";
                midtrans["success"] = false;

                auto response = HttpResponse::newHttpResponse();
                response->setBody(midtrans.dump());
                response->setContentTypeCode(CT_APPLICATION_JSON);
                response->setStatusCode((HttpStatusCode)res.status_code);

                return callback(response);
            }
        }
        catch (const std::exception& e)
        {
            LOG(WARNING) << "Unable to start transaction, error caught on " << e.what();

            Json::Value json;
            json["message"] = fmt::format("Unable to start transaction, error caught on {}", e.what());
            json["success"] = false;

            auto response = HttpResponse::newHttpJsonResponse(json);
            response->setStatusCode(k500InternalServerError);

            return callback(response);
        }
    }
} // namespace gaboot
