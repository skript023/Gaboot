#pragma once
#include <pch.h>

#include "fwddec.hpp"

namespace gaboot
{
    struct item_detail
    {
        int id;
        double price;
        uint64_t quantity;
        std::string name;
    };
}