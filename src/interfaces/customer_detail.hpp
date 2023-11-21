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
    };
}