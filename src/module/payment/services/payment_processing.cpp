#include "payment_processing.hpp"
#include "payments/item_detail.hpp"
#include "payments/customer_detail.hpp"
#include "payments/payment_gataway.hpp"

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
            {"id", item->m_id},
            {"name", item->m_name},
            {"price", item->m_price},
            {"quantity", item->m_quantity}
        };

        auto json = nlohmann::json::array();
        json.push_back(items);

        m_json["item_details"] = json;
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
            {"first_name", customer->m_first_name},
            {"last_name", customer->m_last_name},
            {"email", customer->m_email},
            {"phone", customer->m_phone},
            {"billing_address",
                {"first_name", customer->m_billing_address.m_first_name},
                {"last_name", customer->m_billing_address.m_last_name},
                {"email", customer->m_billing_address.m_email},
                {"phone", customer->m_billing_address.m_phone},
                {"address", customer->m_billing_address.m_address},
                {"city", customer->m_billing_address.m_city},
                {"postal_code", customer->m_billing_address.m_postal_code},
                {"country_code", customer->m_billing_address.m_country_code}
            }
        });
    }
    
    void payment_processing::customer_details(nlohmann::json const& customer)
    {
        if (customer.empty()) return;

        m_json["customer_details"] = customer;
    }

    void payment_processing::start_payment(Json::Value const &json)
    {
        m_transaction.from_json(json);
        m_json = m_transaction.to_json();
    }

    bool payment_processing::make_payment(payment_gateway* midtrans)
    {
        std::string token = fmt::format("Basic {}", SERVER_KEY);

        cpr::Header header = {
            { "Accept", "application/json" },
            { "Content-Type", "application/json" },
            { "Authorization", "Basic U0ItTWlkLXNlcnZlci1GTzllNFFRTlZjVVJmUEYtb2UxMWU5ZFg=" }
        };

        cpr::Body body = m_json.dump();

        LOG(INFO) << "Request payment to " << m_url;

        LOG(INFO) << m_json.dump();

        auto res = cpr::Post(m_url, body, header);

        LOG(INFO) << "Result after request " << res.text;

        if (!res.status_code) throw std::runtime_error("UNKNOWN ERROR 01 - Check your internet access or invalid request");

        if (util::is_json(res.text))
        {
            auto json = nlohmann::ordered_json::parse(res.text);

            json["status_code"] = std::stol(json["status_code"].get<std::string>());

            midtrans->from_json(json);

            if (midtrans->status_code == 201 && res.status_code == 200)
            {
                return true;
            }
        }

        return false;
    }
}