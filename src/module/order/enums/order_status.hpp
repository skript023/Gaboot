#pragma once

namespace gaboot
{
    struct OrderStatus
    {
        static constexpr const char* OPEN = "OPEN";
        static constexpr const char* PENDING_PAYMENT = "PENDING_PAYMENT";
        static constexpr const char* SUCCESS = "SUCCESS";
        static constexpr const char* CANCELLED = "CANCELLED";
        static constexpr const char* EXPIRED = "EXPIRED";
    };
}