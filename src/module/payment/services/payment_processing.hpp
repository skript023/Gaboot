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

        void credit_card(std::string const& orderId, std::string const& tokenId, int grossAmount);
        
        void bank_transfer(std::string const& orderId, std::string const& bankType, int grossAmount);
        
        void electronic_wallet(std::string const& orderId, int grossAmount);

        void item_detail(item_detail* itemDetail);

        void customer_detail(customer_detail* customerDetail);

        bool make_payment(nlohmann::ordered_json& midtrans);
    private:
        nlohmann::json m_json;
        cpr::Url m_url = "https://api.sandbox.midtrans.com/v2/charge";
    };

    inline payment_processing* g_payment_processing{};
}
