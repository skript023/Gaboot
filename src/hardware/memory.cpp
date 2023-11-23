#include "memory.hpp"

namespace gaboot
{
	memory::memory()
	{
        this->get_memory();
        this->get_current_memory();
#ifdef _WIN32
		LOG(INFO) << "Memory used by app : " << this->bytes_to_megabytes(m_process_memory_counter.PrivateUsage) << "MB";
		LOG(INFO) << "Available memory : " << this->bytes_to_gigabytes(m_total_virtual_memory) << " Gigabytes/" << this->bytes_to_gigabytes(m_memory_info.ullTotalPhys - m_memory_info.ullAvailPhys) << " Gigabytes";
#elif __linux__
        LOG(INFO) << "Memory used by app : " << this->kilobytes_to_megabytes(m_personal_usage) << "MB";
        LOG(INFO) << "Available memory : " << this->kilobytes_to_gigabytes(m_total_memory) << " Gigabytes/" << this->kilobytes_to_gigabytes(m_total_memory - m_free_memory) << " Gigabytes";
#endif

		g_virtual_memory = this;
	}
	memory::~memory() noexcept
	{
		g_virtual_memory = nullptr;
	}
    std::string memory::memory_used()
    {
        this->get_memory();
#ifdef _WIN32
        auto current_memory = m_memory_info.ullTotalPhys - m_memory_info.ullAvailPhys;

        return fmt::format("{:.2f}/{:.2f} Gigabytes", this->bytes_to_gigabytes(m_total_virtual_memory), this->bytes_to_gigabytes(current_memory));
#elif __linux__
        auto current_memory = m_total_memory - m_free_memory;

        return fmt::format("{:.2f} Gigabytes", this->bytes_to_gigabytes(current_memory));
#endif
    }
    void memory::get_current_memory()
    {
#ifdef __linux__
        std::ifstream status("/proc/self/status");

        if (status.is_open())
        {
            std::string line;

            while (std::getline(status, line))
            {
                if (line.find("VmRSS:") != std::string::npos)
                {
                    m_personal_usage = parse_memory_info(line);
                    return;
                }
            }
        }
#elif _WIN32
        GetProcessMemoryInfo(GetCurrentProcess(), (PROCESS_MEMORY_COUNTERS*)&m_process_memory_counter, sizeof(m_process_memory_counter));
#endif
    }

    void memory::get_memory()
    {
#ifdef __linux__
        std::ifstream meminfo("/proc/meminfo");

        if (meminfo.is_open())
        {
            std::string line;

            while (std::getline(meminfo, line))
            {
                if (line.find("MemTotal:") != std::string::npos)
                {
                    m_total_memory = parse_memory_info(line);
                }
                else if (line.find("MemFree:") != std::string::npos)
                {
                    m_free_memory = parse_memory_info(line);
                }
            }
        }
#elif _WIN32
        m_memory_info.dwLength = sizeof(MEMORYSTATUSEX);

        GlobalMemoryStatusEx(&m_memory_info);

        m_total_virtual_memory = m_memory_info.ullTotalPhys;
#endif
    }
}