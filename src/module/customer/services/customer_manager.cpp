#include "customer_manager.hpp"

namespace gaboot
{
	customer_manager::~customer_manager()
	{
		g_customer_manager = nullptr;
	}

	customer_manager::customer_manager()
	{
		g_customer_manager = this;
	}

	bool customer_manager::insert(int64_t id)
	{
		try
		{
			auto customer = db().findFutureByPrimaryKey(id).get();
			m_cache_auth.insert({ id, customer });

			return true;
		}
		catch (const DrogonDbException& e)
		{
			LOG(WARNING) << e.base().what();

			return false;
		}
	}

	bool customer_manager::find(int64_t id, MasterCustomers* customer)
	{
		if (auto it = m_cache_auth.find(id); it != m_cache_auth.end())
		{
			*customer = it->second;

			return true;
		}

		LOG(WARNING) << "Customer data not found";

		return false;
	}
	bool customer_manager::remove(int64_t id)
	{
		if (auto it = m_cache_auth.find(id); it != m_cache_auth.end())
		{
			m_cache_auth.erase(it);

			return true;
		}

		return false;
	}
}