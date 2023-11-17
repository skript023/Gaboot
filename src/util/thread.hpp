#pragma once
#include <source_location>

namespace gaboot
{
	// if this limit is hit you did something wrong coding wise.
	constexpr auto MAX_POOL_SIZE = 32u;

	struct thread_pool_job
	{
		std::function<void()> m_func;
		std::source_location m_source_location;
	};

	class thread_pool
	{
		std::atomic<bool> m_accept_jobs;
		std::condition_variable m_data_condition;

		std::stack<thread_pool_job> m_job_stack;
		std::mutex m_lock;
		std::vector<std::thread> m_thread_pool;

		std::atomic<size_t> m_allocated_thread_count;

	public:
		thread_pool(const std::size_t preallocated_thread_count = std::thread::hardware_concurrency());
		~thread_pool();

		void destroy();
		void push(std::function<void()> func, std::source_location location = std::source_location::current());

	private:
		void run();
		void rescale_thread_pool();
	};

	inline thread_pool* g_thread_pool{};
}
