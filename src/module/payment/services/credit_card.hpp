#pragma once

#include <pch.h>

namespace gaboot::credit_card
{
    static nlohmann::json schema = R"(
	{
  	    "payment_type": "credit_card",
  	    "transaction_details": {
    	    "order_id": "order102",
    	    "gross_amount": 789000
  	    },
  	    "credit_card": {
    	    "token_id": "<token_id from Get Card Token Step>",
    	    "authentication": true,
  	    }
        "customer_details": {
            "first_name": "budi",
            "last_name": "pratama",
            "email": "budi.pra@example.com",
            "phone": "08111222333"
        }
    })"_json;
}