#pragma once

#include "util/logger.hpp"

namespace gaboot
{
	using namespace std::chrono;
	class benchmark
	{
	public:
		explicit benchmark(std::string name = "execution") :
			m_start(high_resolution_clock::now()), m_name(name) {}

		void get_runtime()
		{
			auto now = high_resolution_clock::now();
			auto milliseconds_elapsed = duration_cast<milliseconds>(now - m_start);
			auto microseconds_elapsed = duration_cast<microseconds>(now - m_start);
			LOG(INFO) << m_name << " finished with a resulting time of: " << milliseconds_elapsed.count() << "ms " << microseconds_elapsed.count() % 1000 << "us";
		}

		void reset()
		{
			m_start = high_resolution_clock::now();
		}

		static void execute(std::function<void()> cb)
		{
			benchmark bench;
			cb();
			bench.get_runtime();
			bench.reset();
		}
	private:
		high_resolution_clock::time_point m_start;
		std::string m_name;
	};
}

#define BENCHMARK(name, code)\
    {\
        gaboot::benchmark bench(name);\
        code;\
        bench.get_runtime(); \
        bench.reset(); \
    }