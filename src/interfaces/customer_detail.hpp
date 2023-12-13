#pragma once
#include <pch.h>

#include "fwddec.hpp"
#include "billing_address.hpp"
#include "shipping_address.hpp"

namespace gaboot
{
    struct customer_detail
    {
        std::string first_name;
        std::string last_name;
        std::string email;
        std::string phone;
        billing_address m_billing_address;
        shipping_address m_shipping_address;

        void from_json(Json::Value const& json)
        {
            this->first_name = json["customer"]["first_name"].asString();
            this->last_name = json["customer"]["last_name"].asString();
            this->email = json["customer"]["email"].asString();
            this->phone = json["customer"]["phone"].asString();
            this->m_billing_address.first_name = json["customer"]["billing"]["first_name"].asString();
            this->m_billing_address.last_name = json["customer"]["billing"]["last_name"].asString();
            this->m_billing_address.email = json["customer"]["billing"]["email"].asString();
            this->m_billing_address.phone = json["customer"]["billing"]["phone"].asString();
            this->m_billing_address.address = json["customer"]["billing"]["address"].asString();
            this->m_billing_address.city = json["customer"]["billing"]["city"].asString();
            this->m_billing_address.postal_code = json["customer"]["billing"]["postal_code"].asString();
            this->m_billing_address.country_code = json["customer"]["billing"]["country_code"].asString();
        }

        nlohmann::json to_json()
        {
            nlohmann::json json;
            json["first_name"] = first_name;
            json["last_name"] = last_name;
            json["email"] = email;
            json["phone"] = phone;
            json["billing_address"]["first_name"] = m_billing_address.first_name;
            json["billing_address"]["last_name"] = m_billing_address.last_name;
            json["billing_address"]["email"] = m_billing_address.email;
            json["billing_address"]["phone"] = m_billing_address.phone;
            json["billing_address"]["address"] = m_billing_address.address;
            json["billing_address"]["city"] = m_billing_address.city;
            json["billing_address"]["postal_code"] = m_billing_address.postal_code;
            json["billing_address"]["country_code"] = m_billing_address.country_code;

            return json;
        }
    };
}