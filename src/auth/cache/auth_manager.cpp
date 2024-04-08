#include "auth_manager.hpp"

namespace gaboot
{
	auth_manager::~auth_manager()
	{
		g_auth_manager = nullptr;
	}

	auth_manager::auth_manager()
	{
		g_auth_manager = this;
	}

	bool auth_manager::insert(int64_t id)
	{
		try
		{
			if (!this->find(id))
			{
				auto customer = db().findFutureByPrimaryKey(id).get();
				auto result = m_cache_auth.insert({ id, auth_cache(customer, 24h) });

				if (result.second)
					LOG(INFO_TO_FILE) << "Customer " << *result.first->second.m_customers.getUsername() << " successfully inserted into authentication pool";
				else
					LOG(INFO_TO_FILE) << "Customer " << *result.first->second.m_customers.getUsername() << " is already exist in authentication pool";

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

	bool auth_manager::insert(MasterCustomers* customer)
	{
		try
		{
			auto result = m_cache_auth.insert({ customer->getValueOfId(), auth_cache(*customer, 24h) });

			if (result.second)
				LOG(INFO_TO_FILE) << "Customer " << *result.first->second.m_customers.getUsername() << " successfully inserted into authentication pool";
			else
				LOG(INFO_TO_FILE) << "Customer " << *result.first->second.m_customers.getUsername() << " is already exist in authentication pool";

			return true;
		}
		catch (const DrogonDbException& e)
		{
			LOG(WARNING) << e.base().what();

			return false;
		}
	}
	
	bool auth_manager::insert(int64_t id, MasterCustomers* customer)
	{
		try
		{
			if (!this->find(id))
			{
				auto result = m_cache_auth.insert({ id, auth_cache(*customer, 24h) });

				if (result.second)
					LOG(INFO_TO_FILE) << "Customer " << *result.first->second.m_customers.getUsername() << " successfully inserted into authentication pool";
				else
					LOG(INFO_TO_FILE) << "Customer " << *result.first->second.m_customers.getUsername() << " is already exist in authentication pool";

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

	bool auth_manager::find(int64_t id, MasterCustomers* customer)
	{
		if (auto it = m_cache_auth.find(id); it != m_cache_auth.end())
		{
			*customer = it->second.m_customers;

			LOG(INFO_TO_FILE) << "Customer data found";

			return true;
		}

		LOG(WARNING) << "Customer data not found";

		return false;
	}
	MasterCustomers* auth_manager::find(int64_t id)
	{
		if (auto it = m_cache_auth.find(id); it != m_cache_auth.end())
		{
			LOG(INFO_TO_FILE) << "Customer data found";

			return &(it->second.m_customers);
		}

		LOG(WARNING) << "Customer data not found";

		return nullptr;
	}
	std::vector<MasterCustomers> auth_manager::find(std::function<bool(MasterCustomers const&)> callback)
	{
		std::vector<MasterCustomers> result;

		for (auto& entry : m_cache_auth)
		{
			if (callback(entry.second.m_customers))
			{
				result.push_back(entry.second.m_customers);
			}
		}

		return result;
	}
	MasterCustomers* auth_manager::find_one(std::function<bool(MasterCustomers const&)> callback)
	{
		for (auto& entry : m_cache_auth)
		{
			if (callback(entry.second.m_customers))
			{
				return &(entry.second.m_customers);
			}
		}

		return nullptr;
	}
	bool auth_manager::update(int64_t id, MasterCustomers customer)
	{
		if (auto it = m_cache_auth.find(id); it != m_cache_auth.end())
		{
			it->second.m_customers = customer;

			return true;
		}

		return false;
	}
	bool auth_manager::remove(int64_t id)
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