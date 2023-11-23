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
			if (!this->find(id))
			{
				auto customer = db().findFutureByPrimaryKey(id).get();
				auto result = m_cache_auth.insert({ id, customer });

				if (result.second)
					LOG(INFO_TO_FILE) << "Customer " << *result.first->second.getUsername() << " successfully inserted into authentication pool";
				else
					LOG(INFO_TO_FILE) << "Customer " << *result.first->second.getUsername() << " is already exist in authentication pool";

				return true;
			}

			return false;
		}
		catch (const DrogonDbException& e)
		{
			LOG(WARNING) << e.base().what();

			return false;
		}
	}
	
	bool customer_manager::insert(int64_t id, MasterCustomers* customer)
	{
		try
		{
			if (!this->find(id))
			{
				auto result = m_cache_auth.insert({ id, *customer });

				if (result.second)
					LOG(INFO_TO_FILE) << "Customer " << *result.first->second.getUsername() << " successfully inserted into authentication pool";
				else
					LOG(INFO_TO_FILE) << "Customer " << *result.first->second.getUsername() << " is already exist in authentication pool";

				return true;
			}

			return false;
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

			LOG(INFO_TO_FILE) << "Customer data found";

			return true;
		}

		LOG(WARNING) << "Customer data not found";

		return false;
	}
	MasterCustomers* customer_manager::find(int64_t id)
	{
		if (auto it = m_cache_auth.find(id); it != m_cache_auth.end())
		{
			LOG(INFO_TO_FILE) << "Customer data found";

			return &(it->second);
		}

		LOG(WARNING) << "Customer data not found";

		return nullptr;
	}
	bool customer_manager::remove(int64_t id)
	{
		if (auto it = m_cache_auth.find(id); it != m_cache_auth.end())
		{
			auto result = m_cache_auth.erase(it);

			LOG(INFO_TO_FILE) << "Authenticated customer has been cleared at index " << result->first;

			return true;
		}

		LOG(WARNING) << "Failed clear authenticated customer";

		return false;
	}
}