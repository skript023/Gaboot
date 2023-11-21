#pragma once
#include <pch.h>

#include <interfaces/response.hpp>
#include <payment/models/Payments.h>

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
    private:
        response_data m_response;
    };
}