#pragma once
#include <pch.h>
#include <scheduler/schedule.hpp>

namespace gaboot
{
	template<class T>
	class cache_handler
	{
	public:
		void cache_duration(std::chrono::high_resolution_clock::duration time)
		{
			m_duration = std::chrono::high_resolution_clock::now() + time;
		}
		bool insert(std::string id, T* cache)
		{
			if (!this->find(id))
			{
				if (auto result = m_cache.insert({ id, *cache }); !result.second)
					LOG(INFO_TO_FILE) << "Cache is already recorded";

				return true;
			}

			return false;
		}
		bool find(std::string id, T* cache)
		{
			if (auto it = m_cache.find(id); it != m_cache.end())
			{
				*cache = it->second;

				LOG(INFO_TO_FILE) << "Cache found";

				return true;
			}

			LOG(WARNING) << "Cache not found";

			return false;
		}
		T* find(std::string id)
		{
			if (auto it = m_cache.find(id); it != m_cache.end())
			{
				LOG(INFO_TO_FILE) << "Cache found";

				return &(it->second);
			}

			LOG(INFO_TO_FILE) << "Cache not found";

			return nullptr;
		}
		std::vector<T> find_all(size_t limit, size_t offset)
		{
			std::vector<T> result;

			auto start = m_cache.begin();
			std::advance(start, std::min(offset, m_cache.size()));

			auto end = start;
			std::advance(end, std::min(limit, static_cast<int>(m_cache.size()) - offset));

			std::transform(start, end, std::back_inserter(result), [](const auto& pair) { return pair.second; });

			return result;
		}
		std::vector<T> find_all()
		{
			std::vector<T> result;

			auto start = m_cache.begin();
			std::advance(start, std::min(m_offset, m_cache.size()));

			auto end = start;
			std::advance(end, std::min(m_limit, static_cast<int>(m_cache.size()) - m_offset));

			std::transform(start, end, std::back_inserter(result), [](const auto& pair) { return pair.second; });

			return result;
		}
		cache_handler limit(size_t limit)
		{
			m_limit = limit;

			return *this;
		}
		cache_handler offset(size_t offset)
		{
			m_offset = offset;

			return *this;
		}
		bool update(std::string id, T cache)
		{
			if (auto it = m_cache.find(id); it != m_cache.end())
			{
				it->second = cache;

				return true;
			}

			return false;
		}
		bool remove(std::string id)
		{
			if (auto it = m_cache.find(id); it != m_cache.end())
			{
				auto result = m_cache.erase(it);

				LOG(INFO_TO_FILE) << "Cache has been cleared at index " << result->first;

				return true;
			}

			LOG(WARNING) << "Failed clear cache";

			return false;
		}
		void cleanup()
		{
			if (m_duration <= std::chrono::high_resolution_clock::now())
			{
				m_cache.clear();
			}
		}
		void clear()
		{
			m_cache.clear();
		}
		std::string first_id()
		{
			auto first = m_cache.begin();

			return first->first;
		}
		std::string last_id()
		{
			auto last = --m_cache.end();

			return last->first;
		}
		size_t size()
		{
			return m_cache.size();
		}
		bool empty() { return m_cache.empty(); }
		bool expired()
		{
			if (m_duration <= std::chrono::high_resolution_clock::now())
			{
				m_cache.clear();

				LOG(INFO) << "Cache expired";

				return true;
			}

			return false;
		}

		void cache_duration(std::chrono::high_resolution_clock::duration time) const
		{
			m_duration = std::chrono::high_resolution_clock::now() + time;
		}
		bool insert(std::string id, T* cache) const
		{
			if (!this->find(id))
			{
				if (auto result = m_cache.insert({ id, *cache }); !result.second)
					LOG(INFO_TO_FILE) << "Cache is already recorded";

				return true;
			}

			return false;
		}
		bool find(std::string id, T* cache) const
		{
			if (auto it = m_cache.find(id); it != m_cache.end())
			{
				*cache = it->second;

				LOG(INFO_TO_FILE) << "Cache found";

				return true;
			}

			LOG(INFO_TO_FILE) << "Cache not found";

			return false;
		}
		T* find(std::string id) const
		{
			if (auto it = m_cache.find(id); it != m_cache.end())
			{
				LOG(INFO_TO_FILE) << "Cache found";

				return &(it->second);
			}

			LOG(INFO_TO_FILE) << "Cache not found";

			return nullptr;
		}
		std::vector<T> find(std::function<bool(const T&)> callback)
		{
			std::vector<T> result;

			for (auto& entry : m_cache)
			{
				if (callback(entry.second))
				{
					result.push_back(entry.second);
				}
			}

			return result;
		}
		bool update(std::string id, T cache) const
		{
			if (auto it = m_cache.find(id); it != m_cache.end())
			{
				it->second = cache;

				return true;
			}

			return false;
		}
		bool remove(std::string id) const
		{
			if (auto it = m_cache.find(id); it != m_cache.end())
			{
				auto result = m_cache.erase(it);

				LOG(INFO_TO_FILE) << "Cache has been cleared at index " << result->first;

				return true;
			}

			LOG(WARNING) << "Failed clear cache";

			return false;
		}
		void cleanup() const
		{
			if (m_duration <= std::chrono::high_resolution_clock::now())
			{
				m_cache.clear();
			}
		}
		void clear() const
		{
			m_cache.clear();
		}
		std::string first_id() const
		{
			auto first = m_cache.begin();

			return first->first;
		}
		std::string last_id() const
		{
			auto last = --m_cache.end();

			return last->first;
		}
		size_t size() const
		{
			return m_cache.size();
		}
		bool empty() const { return m_cache.empty(); }
		bool expired() const
		{
			if (m_duration <= std::chrono::high_resolution_clock::now())
			{
				m_cache.clear();

				return true;
			}

			return false;
		}
	private:
		size_t m_limit;
		size_t m_offset;
		std::map<std::string, T>m_cache;
		std::chrono::high_resolution_clock::time_point m_duration;
	};
}