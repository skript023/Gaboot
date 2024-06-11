#pragma once
#include <pch.h>
#include <payment/models/Payments.h>

#include <interfaces/response.hpp>
#include <payments/payment_gataway.hpp>
#include <payments/item_detail.hpp>
#include <payments/customer_detail.hpp>

using namespace drogon;
using namespace orm;
using namespace drogon_model::gaboot;

namespace gaboot
{
    class payment_service
    {
    public:
        explicit payment_service();
        ~payment_service() noexcept;

        payment_service(payment_service const&) = delete;
        payment_service& operator=(payment_service const&) = delete;

        payment_service(payment_service&&) = delete;
        payment_service& operator=(payment_service&&) = delete;

        HttpResponsePtr create(HttpRequestPtr const&);
        HttpResponsePtr findOne(HttpRequestPtr const&, std::string&& transactionId);
        HttpResponsePtr callback(HttpRequestPtr const&);
    private:
        std::unique_ptr<response_data<payment_gateway>> m_response;
        std::unique_ptr<Mapper<Payments>> m_database;
        customer_detail m_customer;
        Json::Value m_data;
        item_detail m_items;
    };
}