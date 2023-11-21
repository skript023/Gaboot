#pragma once

#include <pch.h>
#include <util/validator.hpp>
#include <util/jenkins.hpp>
#include <interfaces/fwddec.hpp>

namespace gaboot
{
    class payment_processing final
    {
    public:
        explicit payment_processing();

        ~payment_processing();

        payment_processing credit_card(std::string const& orderId, std::string const& tokenId, int grossAmount);
        
        payment_processing bank_transfer(std::string const& orderId, std::string const& bankType, int grossAmount);
        
        payment_processing electronic_wallet(std::string const& orderId, int grossAmount);

        payment_processing item_detail(item_detail* itemDetail);

        payment_processing customer_detail(customer_detail* customerDetail);

        bool make_payment(nlohmann::ordered_json& midtrans);
    private:
        std::string m_order_id;
        std::string m_token_id;
        std::string m_bank;
        int m_gross_amount;
    private:
        nlohmann::json m_json;
        cpr::Body m_body;
        cpr::Url m_url = "https://api.sandbox.midtrans.com/v2/charge";
    };

    inline payment_processing* g_payment_processing{};
}
