#include "payment_service.hpp"

namespace gaboot
{
    payment_service::payment_service(std::string const& order_id, int gross_amount, std::string token_id) :
        m_order_id(order_id), m_gross_amount(gross_amount), m_token_id(token_id)
    {};

    payment_service::payment_service(std::string const& order_id, std::string bank, int gross_amount) :
        m_order_id(order_id), m_gross_amount(gross_amount), m_bank(bank)
    {};
    
    payment_service::payment_service(nlohmann::json const& params) :
        m_body(params.dump()), m_json(params)
    {};

    payment_service payment_service::credit_card()
    {
        m_json["payment_type"] = "credit_card";
        m_json["transaction_details"]["order_id"] = m_order_id;
        m_json["transaction_details"]["gross_amount"] = m_gross_amount;

        m_json["credit_card"]["token_id"] = m_token_id;
        m_json["credit_card"]["authentication"] = true;

        return payment_service(m_json);
    }

    payment_service payment_service::bank_transfer()
    {
        m_json["payment_type"] = "bank_transfer";
        m_json["transaction_details"]["order_id"] = m_order_id;
        m_json["transaction_details"]["gross_amount"] = m_gross_amount;

        m_json["bank_transfer"]["bank"] = m_bank;

        return payment_service(m_json);
    }

    payment_service payment_service::electronic_wallet()
    {
        m_json["payment_type"] = "gopay";
        m_json["transaction_details"]["order_id"] = m_order_id;
        m_json["transaction_details"]["gross_amount"] = m_gross_amount;

        return payment_service(m_json);
    }

    bool payment_service::make_payment(nlohmann::json& midtrans)
    {
        std::string token = fmt::format("Basic {}", SERVER_KEY);

        cpr::Header header = {
            {"Accept", "application/json"},
            { "Content-Type", "application/json" },
            { "Authorization", token}
        };

        auto async = cpr::PostAsync(m_url, m_body, header);
        auto res = async.get();

        if (res.status_code == 201)
        {
            midtrans = nlohmann::ordered_json::parse(res.text);

            return true;
        }

        return false;
    }
}