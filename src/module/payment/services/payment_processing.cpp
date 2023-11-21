#include "payment_processing.hpp"

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

    payment_processing payment_processing::credit_card(std::string orderId, int grossAmount, std::string tokenId)
    {
        m_json["payment_type"] = "credit_card";
        m_json["transaction_details"]["order_id"] = std::move(orderId);
        m_json["transaction_details"]["gross_amount"] = grossAmount;

        m_json["credit_card"]["token_id"] = std::move(tokenId);
        m_json["credit_card"]["authentication"] = true;

        return *this;
    }

    payment_processing payment_processing::bank_transfer(std::string orderId, std::string bankType, int grossAmount)
    {
        m_json["payment_type"] = "bank_transfer";
        m_json["transaction_details"]["order_id"] = std::move(orderId);
        m_json["transaction_details"]["gross_amount"] = grossAmount;

        m_json["bank_transfer"]["bank"] = std::move(bankType);

        return *this;
    }

    payment_processing payment_processing::electronic_wallet(std::string orderId, int grossAmount)
    {
        m_json["payment_type"] = "gopay";
        m_json["transaction_details"]["order_id"] = std::move(orderId);
        m_json["transaction_details"]["gross_amount"] = grossAmount;

        return *this;
    }

    bool payment_processing::make_payment(nlohmann::json& midtrans)
    {
        std::string token = fmt::format("Basic {}", SERVER_KEY);

        cpr::Header header = {
            {"Accept", "application/json"},
            { "Content-Type", "application/json" },
            { "Authorization", token}
        };

        m_body = m_json.dump();

        auto res = cpr::PostAsync(m_url, m_body, header).get();

        if (res.status_code == 200)
        {
            midtrans = nlohmann::ordered_json::parse(res.text);

            return true;
        }

        return false;
    }
}