#pragma once

#include <pch.h>
#include "module/customer/models/MasterCustomers.h"

using namespace drogon::orm;
using namespace drogon_model::gaboot;

namespace gaboot
{
	class customer_manager
	{
		Mapper<MasterCustomers> db() { return Mapper<MasterCustomers>(DATABASE_CLIENT); }
	public:
		explicit customer_manager();
		virtual ~customer_manager();

		bool insert(int64_t id);
		bool find(int64_t id, MasterCustomers* customer);
		bool remove(int64_t id);
	private:
		std::map<int64_t, MasterCustomers> m_cache_auth;
	};

	inline customer_manager* g_customer_manager;
}