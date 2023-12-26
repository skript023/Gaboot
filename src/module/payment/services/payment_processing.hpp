#pragma once

#include <pch.h>
#include <validator/validator.hpp>
#include <hash/jenkins.hpp>
#include <interfaces/fwddec.hpp>

namespace gaboot
{
    class payment_processing final
    {
    public:
        explicit payment_processing();

        ~payment_processing();

        void credit_card(std::string const& orderId, std::string const& tokenId, int grossAmount);
        
        void bank_transfer(std::string const& orderId, std::string const& bankType, int grossAmount);
        
        void electronic_wallet(std::string const& orderId, int grossAmount);

        void item_details(item_detail* itemDetail);

        void item_details(nlohmann::json const& itemDetail);

        void customer_details(customer_detail* customerDetail);

        void customer_details(nlohmann::json const& customerDetail);

        bool make_payment(nlohmann::ordered_json& midtrans);
    private:
        nlohmann::json m_json;
        cpr::Url m_url = "https://api.sandbox.midtrans.com/v2/charge";
    };

    inline payment_processing* g_payment_processing{};
}
