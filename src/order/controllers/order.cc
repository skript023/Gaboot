#include "order.h"

// Add definition of your processing function here
namespace gaboot
{
    void order::create(HttpRequest const& req, response_t&& callback)
    {
        std::string order_id;
        std::string first_name;
        std::string last_name;
        std::string email;
        std::string phone;
        std::string token = std::format("Basic ", CLIENT_KEY);

        std::regex pattern(R"(\d{3}-\d{3}-\d{4})");

        nlohmann::json json = {
            {
                "transaction_detail", {
                    {"order_id", order_id}
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
        cpr::Body body = json.dump();
        cpr::Header header {
            { "Content-Type", "application/json" },
            { "Authorization", token}
        };

        auto async = cpr::PostAsync(url, body, header);
        
        auto res = async.get();
        auto midtrans = nlohmann::json::parse(res.text);

        Json::Value resp;
        resp["token"] = midtrans["token"].get<std::string>();

        auto response = HttpResponse::newHttpJsonResponse(resp);

        callback(response);
    }
} // namespace gaboot
