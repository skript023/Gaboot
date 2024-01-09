#pragma once

#include <pch.h>

namespace gaboot
{
	struct midtrans_response
	{
		std::string m_currency;
		std::string m_expiry_time;
		std::string m_merchant_id;
		std::string m_payment_type;
		std::string m_transaction_id;
		std::string m_transaction_status;
		std::string m_transaction_time;
		std::string m_fraud_status;
	}; 
}