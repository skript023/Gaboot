#pragma once

namespace gaboot::example
{
	inline void example_generate_payment()
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
	}
}