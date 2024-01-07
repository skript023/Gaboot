#pragma once

#include <customer_detail.hpp>
#include <order.hpp>

namespace gaboot
{
	struct transaction
	{
		std::string payment_type;
		order transaction_details;
		customer_detail customer_detail;
	};
}