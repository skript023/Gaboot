#pragma once
#include "formatter/money_formatter.hpp"

namespace gaboot
{
    template<class T>
    class response_data;
    struct customer_detail;
    struct item_detail;
    struct shipping_address;
    struct billing_address;
    struct shipping_address;
    struct payment_gateway;

    using Rupiah = money_formatter;
}