#pragma once

#include <pch.h>
#include <util/validator.hpp>
#include <util/jenkins.hpp>

namespace gaboot
{
    class payment_service
    {
    public:
        explicit payment_service(nlohmann::json const& params) :
            m_body(params.dump()), m_json(params)
        {};

        virtual ~payment_service() = default;

        bool make_payment(nlohmann::json& midtrans)
        {
            std::string token = fmt::format("Basic {}", SERVER_KEY);

            cpr::Header header = {
                {"Accept", "application/json"},
                { "Content-Type", "application/json" },
                { "Authorization", token}
            };

            auto async = cpr::PostAsync(m_url, m_body, header);
            auto res = async.get();

            if (res.status_code == 201)
            {
                midtrans = nlohmann::ordered_json::parse(res.text);

                return true;
            }

            return false;
        }
    private:
        nlohmann::json m_json;
        cpr::Body m_body;
        cpr::Url m_url = "https://api.sandbox.midtrans.com/v2/charge";
    };
} // namespace gaboot
