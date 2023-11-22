#include "payment_processing.hpp"
#include "thread.hpp"

#include "interfaces/item_detail.hpp"
#include "interfaces/customer_detail.hpp"

namespace gaboot
{
    payment_processing::payment_processing()
    {
        g_payment_processing = this;
    };

    payment_processing::~payment_processing()
    {
        g_payment_processing = nullptr;
    }

    void payment_processing::credit_card(std::string const& orderId, std::string const& tokenId, int grossAmount)
    {
        m_json["payment_type"] = "credit_card";
        m_json["transaction_details"]["order_id"] = orderId;
        m_json["transaction_details"]["gross_amount"] = grossAmount;

        m_json["credit_card"]["token_id"] = tokenId;
        m_json["credit_card"]["authentication"] = true;
    }

    void payment_processing::bank_transfer(std::string const& orderId, std::string const& bankType, int grossAmount)
    {
        m_json["payment_type"] = "bank_transfer";
        m_json["transaction_details"]["order_id"] = orderId;
        m_json["transaction_details"]["gross_amount"] = grossAmount;

        m_json["bank_transfer"]["bank"] = bankType;
    }

    void payment_processing::electronic_wallet(std::string const& orderId, int grossAmount)
    {
        m_json["payment_type"] = "gopay";
        m_json["transaction_details"]["order_id"] = orderId;
        m_json["transaction_details"]["gross_amount"] = grossAmount;
    }

    void payment_processing::item_details(item_detail* itemDetail)
    {
        auto items = nlohmann::json::object({
            {"id", itemDetail->id},
            {"name", itemDetail->name},
            {"price", itemDetail->price},
            {"quantity", itemDetail->quantity}
        });

        m_json["item_details"] = nlohmann::json::array({ items });
    }

    void payment_processing::customer_details(customer_detail* customerDetail)
    {
        m_json["customer_detail"] = nlohmann::json::object({
            {"first_name", customerDetail->first_name},
            {"last_name", customerDetail->last_name},
            {"email", customerDetail->email},
            {"phone", customerDetail->phone},
        });
    }

    bool payment_processing::make_payment(nlohmann::ordered_json& midtrans)
    {
        std::string token = fmt::format("Basic {}", SERVER_KEY);

        cpr::Header header = {
            { "Accept", "application/json" },
            { "Content-Type", "application/json" },
            { "Authorization", token }
        };

        cpr::Body body = m_json.dump();

        auto res = cpr::PostAsync(m_url, body, header).get();

        auto json = nlohmann::ordered_json::parse(res.text);

        midtrans = json;

        if (json["status_code"] == "201" && res.status_code == 200)
        {
            return true;
        }

        return false;
    }
}