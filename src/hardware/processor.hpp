#pragma once
#include <pch.h>

namespace gaboot
{
	class processor
	{
	public:
		explicit processor();
		~processor() noexcept;

		double process_cpu_usage();
		std::string processor_utilization();
		void read_stat_file();
	private:
#ifdef _WIN32
		ULARGE_INTEGER m_last_cpu, m_last_sys_cpu, m_last_user_cpu;
		int m_num_processor;
		HANDLE m_self;
	private:
		PDH_HQUERY m_query;
		PDH_HCOUNTER m_counter;
#elif __linux__
		unsigned long long m_total_user_time;
		unsigned long long m_total_sys_time;
		unsigned long long m_prev_total_user_time;
		unsigned long long m_prev_total_sys_time;
		unsigned long long m_prev_total_time;

		std::string m_cpu_label;
		unsigned long long m_total_nice_time;
		unsigned long long m_total_idle_time;
		unsigned long long m_total_io_wait_time;
		unsigned long long m_total_iqrt_time;
		unsigned long long m_total_soft_iqrt_time;
#endif
	};

	inline processor* g_processor{};
}