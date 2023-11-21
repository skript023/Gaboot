#pragma once
#include <pch.h>

#include "fwddec.hpp"

namespace gaboot
{
    struct shipping_address
    {
        std::string first_name;
        std::string last_name;
        std::string email;
        std::string phone;
        std::string address;
        std::string city;
        std::string postal_code;
        std::string country_code;
    };
}