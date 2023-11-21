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

        payment_processing credit_card(std::string orderId, int grossAmount, std::string tokenId);
        
        payment_processing bank_transfer(std::string orderId, std::string bankType, int grossAmount);
        
        payment_processing electronic_wallet(std::string orderId, int grossAmount);

        payment_processing item_detail(item_detail* itemDetail);

        payment_processing customer_detail(customer_detail* customerDetail);

        bool make_payment(nlohmann::json& midtrans);
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
