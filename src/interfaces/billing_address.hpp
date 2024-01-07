#pragma once
#include <pch.h>

#include "fwddec.hpp"

namespace gaboot
{
    struct billing_address
    {
        std::string m_first_name;
        std::string m_last_name;
        std::string m_email;
        std::string m_phone;
        std::string m_address;
        std::string m_city;
        std::string m_postal_code;
        std::string m_country_code;
    };
}