#pragma once

#include <pch.h>
#include <util/validator.hpp>
#include <util/jenkins.hpp>

namespace gaboot
{
    class payment_service final
    {
    public:
        payment_service(std::string const& order_id, int gross_amount, std::string token_id);

        payment_service(std::string const& order_id, std::string bank, int gross_amount);

        payment_service(nlohmann::json const& params);

        virtual ~payment_service() = default;

        payment_service credit_card();
        
        payment_service bank_transfer();
        
        payment_service electronic_wallet();

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
} // namespace gaboot
