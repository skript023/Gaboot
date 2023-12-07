#pragma once
#include <pch.h>

#include "fwddec.hpp"

namespace gaboot
{
    struct customer_detail
    {
        std::string first_name;
        std::string last_name;
        std::string email;
        std::string phone;
        billing_address* m_billing_address;
        shipping_address* m_shipping_address;

        void from_json(Json::Value const& json)
        {
            this->first_name = json["customer"]["first_name"].asString();
            this->last_name = json["customer"]["last_name"].asString();
            this->email = json["customer"]["email"].asString();
            this->phone = json["customer"]["phone"].asString();
        }

        nlohmann::json to_json()
        {
            nlohmann::json json;
            json["first_name"] = first_name;
            json["last_name"] = last_name;
            json["email"] = email;
            json["phone"] = phone;

            return json;
        }
    };
}