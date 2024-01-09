#pragma once

#include <pch.h>
#include <hash/jenkins.hpp>
#include <interfaces/fwddec.hpp>
#include <validator/validator.hpp>
#include <interfaces/transaction.hpp>

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

        void start_payment(Json::Value const& json);

        bool make_payment(nlohmann::ordered_json& midtrans);
    private:
        nlohmann::json m_json;
        transaction m_transaction;
#ifdef _DEV
        cpr::Url m_url = "https://api.sandbox.midtrans.com/v2/charge";
#elif _PROD
        cpr::Url m_url = "https://api.midtrans.com/v2/charge";
#endif
    };

    inline payment_processing* g_payment_processing{};

    #define TRANSACTION_BEGIN_CLAUSE(json, midtrans) g_payment_processing->start_payment(json); if (g_payment_processing->make_payment(midtrans)) {
    #define TRANSACTION_END_CLAUSE }
    #define TRANSACTION_FAILED(midtrans) auto response = HttpResponse::newHttpResponse(); response->setStatusCode((HttpStatusCode)midtrans["status_code"].get<int>()); response->setContentTypeCode(CT_APPLICATION_JSON); response->setBody(midtrans.dump()); return response;
    #define TRANSACTION_SUCCESS(midtrans) auto response = HttpResponse::newHttpResponse(); response->setContentTypeCode(CT_APPLICATION_JSON); response->setBody(midtrans.dump()); response->setStatusCode(k201Created); return response;
}
