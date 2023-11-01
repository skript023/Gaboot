#include "pch.h"
#include "schedule.hpp"

namespace gaboot
{
	schedule::schedule()
	{
		g_schedule = this;
	}

	schedule::~schedule()
	{
		g_schedule = nullptr;
	}

	void schedule::call()
	{
		std::chrono::duration<double> duration = m_wake_time - std::chrono::high_resolution_clock::now();

		std::unique_lock lock(m_mutex);
		if (!m_tasks.empty())
		{
			scheduler_t job = std::move(m_tasks.top());
			m_tasks.pop();
			lock.unlock();

			RUN_EVERY(duration, std::move(job));
		}
	}

	schedule* schedule::task(scheduler_t callback)
	{
		if (callback)
		{
			std::lock_guard lock(m_mutex);
			m_tasks.push(std::move(callback));
		}

		return this;
	}

	schedule* schedule::every(std::chrono::high_resolution_clock::duration time)
	{
		m_wake_time = std::chrono::high_resolution_clock::now() + time;

		return this;
	}
}