#pragma once

#include <pch.h>
#include "module/customer/models/MasterCustomers.h"

using namespace drogon::orm;
using namespace drogon_model::gaboot;

namespace gaboot
{
	struct auth_cache
	{
		auth_cache(MasterCustomers customer, std::chrono::high_resolution_clock::duration duration):
			m_customers(customer), m_duration(std::chrono::high_resolution_clock::now() + duration) { }

		std::chrono::high_resolution_clock::time_point m_duration;
		MasterCustomers m_customers;
	};

	class auth_manager
	{
		Mapper<MasterCustomers> db() { return Mapper<MasterCustomers>(DATABASE_CLIENT); }
	public:
		explicit auth_manager();
		virtual ~auth_manager();

		bool insert(int64_t id);
		bool insert(MasterCustomers* customer);
		bool insert(int64_t id, MasterCustomers* customer);
		bool find(int64_t id, MasterCustomers* customer);
		MasterCustomers* find(int64_t id);
		std::vector<MasterCustomers> find(std::function<bool(MasterCustomers const&)> callback);
		MasterCustomers* find_one(std::function<bool(MasterCustomers const&)> callback);
		bool update(int64_t id, MasterCustomers customer);
		bool remove(int64_t id);
	private:
		std::map<int64_t, auth_cache> m_cache_auth;
	};

	inline auth_manager* g_auth_manager;
}