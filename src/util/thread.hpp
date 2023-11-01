#pragma once

namespace gaboot
{
	// if this limit is hit you did something wrong coding wise.
	constexpr auto MAX_POOL_SIZE = 32u;

	class thread_pool
	{
		std::function<void()> m_func;
		std::atomic<bool> m_accept_jobs;
		std::condition_variable m_data_condition;

		std::stack<std::function<void()>> m_job_stack;
		std::mutex m_lock;
		std::vector<std::thread> m_thread_pool;

		std::atomic<size_t> m_allocated_thread_count;

	public:
		// YimMenu only has 2 blocking threads, 4 should be sufficient but the pool should automatically allocate more if needed
		thread_pool(const std::size_t preallocated_thread_count = 4);
		~thread_pool();

		void destroy();
		void push(std::function<void()> func);

	private:
		void run();
		void rescale_thread_pool();
	};

	inline thread_pool* g_thread_pool{};
}