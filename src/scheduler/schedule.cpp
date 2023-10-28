#include "pch.h"
#include "schedule.hpp"

namespace gaboot
{
	schedule::schedule()
	{
		drogon::app().getLoop()->runEvery(1ms, schedule::run);

		g_schedule = this;
	}

	schedule::~schedule()
	{
		g_schedule = nullptr;
	}

	void schedule::tick()
	{
		if (!m_wake_time.has_value() || m_wake_time.value() <= std::chrono::high_resolution_clock::now())
		{
			std::unique_lock lock(m_mutex);
			if (!m_jobs.empty())
			{
				std::function<void()> job = std::move(m_jobs.top());
				m_jobs.pop();
				lock.unlock();

				std::invoke(std::move(job));
			}
		}
	}

	schedule* schedule::task(std::function<void()> job)
	{
		if (job)
		{
			std::lock_guard lock(m_mutex);
			m_jobs.push(std::move(job));
		}

		return this;
	}

	void schedule::every(std::optional<std::chrono::high_resolution_clock::duration> time)
	{
		if (time.has_value())
		{
			m_wake_time = std::chrono::high_resolution_clock::now() + time.value();
		}
		else
		{
			m_wake_time = std::nullopt;
		}
	}

	void schedule::run()
	{
		g_schedule->tick();
	}
}