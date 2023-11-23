#pragma once
#include <pch.h>

namespace gaboot
{
	class memory
	{
	public:
		explicit memory();
		~memory() noexcept;

		std::string memory_used();
		void get_current_memory();
		void get_memory();

		double bytes_to_gigabytes(unsigned long long bytes)  { return static_cast<double>(bytes) / (1024 * 1024 * 1024); }
		double bytes_to_megabytes(unsigned long long bytes)  { return static_cast<double>(bytes) / (1024 * 1024); }
		double bytes_to_kilobytes(unsigned long long bytes)  { return static_cast<double>(bytes) / 1024; }

		double kilobytes_to_gigabytes(unsigned long long kilobytes) { return static_cast<double>(kilobytes) / (1024 * 1024); }
		double kilobytes_to_megabytes(unsigned long long kilobytes) { return static_cast<double>(kilobytes) / 1024; }
		double kilobytes_to_bytes(unsigned long long kilobytes) { return static_cast<double>(kilobytes) * 1024; }
	private:
#ifdef _WIN32
		MEMORYSTATUSEX m_memory_info;
		DWORDLONG m_total_virtual_memory;
		PROCESS_MEMORY_COUNTERS_EX m_process_memory_counter;
#elif __linux__
		unsigned long long m_total_memory;
		unsigned long long m_free_memory;
		unsigned long long m_personal_usage;

		unsigned long long parse_memory_info(const std::string& line) const 
		{
			size_t startPos = line.find_first_of("0123456789");
			size_t endPos = line.find_last_of("0123456789");

			return std::stoull(line.substr(startPos, endPos - startPos + 1));
		}
#endif
	};

	inline memory* g_virtual_memory{};
}