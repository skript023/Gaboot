#pragma once

namespace gaboot
{
	using scheduler_t = std::function<void()>;

	class schedule
	{
	public:
		explicit schedule();
		virtual ~schedule() noexcept;

		schedule(schedule const& that) = delete;
		schedule& operator=(schedule const& that) = delete;
		schedule(schedule&& that) = delete;
		schedule& operator=(schedule&& that) = delete;

		void call();
		schedule* task(scheduler_t job);
		schedule* every(std::chrono::high_resolution_clock::duration time);
	private:
		std::chrono::high_resolution_clock::time_point m_wake_time;
		std::recursive_mutex m_mutex;
		std::stack<scheduler_t> m_tasks;
	};

	inline schedule* g_schedule;
}