#pragma once
#include <pch.h>
#include <payment/models/Payments.h>

#include <interfaces/fwddec.hpp>
#include <interfaces/response.hpp>
#include <payments/item_detail.hpp>
#include <payments/customer_detail.hpp>

using namespace drogon;
using namespace orm;
using namespace drogon_model::gaboot;

namespace gaboot
{
    class payment_service
    {
        Mapper<Payments> db() { return Mapper<Payments>(DATABASE_CLIENT); }
    public:
        payment_service() = default;
        ~payment_service() noexcept = default;

        payment_service(payment_service const&) = delete;
        payment_service& operator=(payment_service const&) = delete;

        payment_service(payment_service&&) = delete;
        payment_service& operator=(payment_service&&) = delete;

        HttpResponsePtr create(HttpRequestPtr const&);
        HttpResponsePtr findOne(HttpRequestPtr const&, std::string&& transactionId);
        HttpResponsePtr callback(HttpRequestPtr const&);
    private:
        Json::Value m_data;
        response_data m_response;
        item_detail m_items;
        customer_detail m_customer;
    };
}