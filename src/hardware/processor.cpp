#include "processor.hpp"

namespace gaboot
{
	processor::processor()
	{
#ifdef _WIN32
        PdhOpenQuery(nullptr, 0, &m_query);
        PdhAddCounter(m_query, "\\Processor(_Total)\\% Processor Time", 0, &m_counter);
        PdhCollectQueryData(m_query);

        SYSTEM_INFO sysInfo;
        FILETIME ftime, fsys, fuser;

        GetSystemInfo(&sysInfo);
        m_num_processor = sysInfo.dwNumberOfProcessors;

        GetSystemTimeAsFileTime(&ftime);
        memcpy(&m_last_cpu, &ftime, sizeof(FILETIME));

        m_self = GetCurrentProcess();
        GetProcessTimes(m_self, &ftime, &ftime, &fsys, &fuser);
        memcpy(&m_last_sys_cpu, &fsys, sizeof(FILETIME));
        memcpy(&m_last_user_cpu, &fuser, sizeof(FILETIME));

#endif
        LOG(INFO) << "CPU Usage : " << this->processor_utilization();

        g_processor = this;
	}
    processor::~processor() noexcept
    {
#ifdef _WIN32
        PdhCloseQuery(m_query);
#endif

        g_processor = nullptr;
    }
#ifdef _WIN32
    double processor::process_cpu_usage()
    {
        FILETIME ftime, fsys, fuser;
        ULARGE_INTEGER now, sys, user;
        double percent;

        GetSystemTimeAsFileTime(&ftime);
        memcpy(&now, &ftime, sizeof(FILETIME));

        GetProcessTimes(m_self, &ftime, &ftime, &fsys, &fuser);
        memcpy(&sys, &fsys, sizeof(FILETIME));
        memcpy(&user, &fuser, sizeof(FILETIME));

        percent = static_cast<double>(sys.QuadPart - m_last_sys_cpu.QuadPart) + static_cast<double>(user.QuadPart - m_last_user_cpu.QuadPart);
        percent /= static_cast<double>(now.QuadPart - m_last_cpu.QuadPart);
        percent /= m_num_processor;

        m_last_cpu = now;
        m_last_user_cpu = user;
        m_last_sys_cpu = sys;

        return percent / 100;
}
#endif
    std::string processor::processor_utilization()
    {
#ifdef _WIN32
        PdhCollectQueryData(m_query);
        PDH_FMT_COUNTERVALUE value;
        PdhGetFormattedCounterValue(m_counter, PDH_FMT_DOUBLE, nullptr, &value);

        return fmt::format("{:.2f}%", value.doubleValue);
#elif __linux__
        this->read_stat_file();

        double totalTime = static_cast<double>(m_total_user_time + m_total_sys_time);
        double elapsedTime = static_cast<double>(totalTime - m_prev_total_time);

        double usagePercentage = (elapsedTime > 0) ? ((m_total_user_time + m_total_sys_time - m_prev_total_user_time - m_prev_total_sys_time) / elapsedTime) * 100 : 0;

        m_prev_total_time = totalTime;
        m_prev_total_user_time = m_total_user_time;
        m_prev_total_sys_time = m_total_sys_time;

        return fmt::format("{:.2f}%", usagePercentage);
#endif
    }
#ifdef __linux__
    void processor::read_stat_file()
    {
        std::ifstream stat_file("/proc/stat");

        if (stat_file.is_open())
        {
            std::string line;
            std::getline(stat_file, line);

            if (line.compare(0, 3, "cpu") == 0)
            {
                std::istringstream iss(line);
                iss >> m_cpu_label >> m_total_user_time >> m_total_nice_time >> m_total_sys_time >> m_total_idle_time;

                while (iss >> m_total_io_wait_time >> m_total_iqrt_time >> m_total_soft_iqrt_time)
                {
                }
            }
        }
    }
#endif
}