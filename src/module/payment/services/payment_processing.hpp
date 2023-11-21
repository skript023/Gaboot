#pragma once

#include <pch.h>
#include <util/validator.hpp>
#include <util/jenkins.hpp>

namespace gaboot
{
    class payment_processing final
    {
    public:
        payment_processing(std::string const& order_id, int gross_amount, std::string token_id);

        payment_processing(std::string const& order_id, std::string bank, int gross_amount);

        payment_processing(nlohmann::json const& params);

        payment_processing() = default;

        ~payment_processing() = default;

        payment_processing credit_card();
        
        payment_processing bank_transfer();
        
        payment_processing electronic_wallet();

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
