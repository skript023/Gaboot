#include "payment_processing.hpp"
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

    void payment_processing::item_details(item_detail* item)
    {
        if (!item) return;

        nlohmann::json items = {
            {"id", item->id},
            {"name", item->name},
            {"price", item->price},
            {"quantity", item->quantity}
        };

        m_json["item_details"] = nlohmann::json::array({ items });
    }
    
    void payment_processing::item_details(nlohmann::json const& item)
    {
        if (item.empty()) return;

        m_json["item_details"] = item;
    }

    void payment_processing::customer_details(customer_detail* customer)
    {
        if (!customer) return;

        m_json["customer_details"] = nlohmann::json({
            {"first_name", customer->first_name},
            {"last_name", customer->last_name},
            {"email", customer->email},
            {"phone", customer->phone},
            {"billing_address",
                {"first_name", customer->m_billing_address.first_name},
                {"last_name", customer->m_billing_address.last_name},
                {"email", customer->m_billing_address.email},
                {"phone", customer->m_billing_address.phone},
                {"address", customer->m_billing_address.address},
                {"city", customer->m_billing_address.city},
                {"postal_code", customer->m_billing_address.postal_code},
                {"country_code", customer->m_billing_address.country_code}
            }
        });
    }
    
    void payment_processing::customer_details(nlohmann::json const& customer)
    {
        if (customer.empty()) return;

        m_json["customer_details"] = customer;
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

        json["status_code"] = std::stol(json["status_code"].get<std::string>());

        midtrans = json;

        if (json["status_code"] == 201 && res.status_code == 200)
        {
            return true;
        }

        return false;
    }
}