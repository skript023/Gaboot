#pragma once
#include <pch.h>

#include "hardware/memory.hpp"
#include "hardware/processor.hpp"

namespace gaboot
{
	struct PerformanceResponse
	{
		PerformanceResponse() = default;

		PerformanceResponse(int core = std::thread::hardware_concurrency(), std::string cpu_usage = g_processor->processor_utilization(), std::string memory = g_virtual_memory->memory_used()) :
			total_core(core),
			cpu_usage(cpu_usage),
			memory_usage(memory)
		{

		}

		int total_core;
		std::string cpu_usage;
		std::string memory_usage;

		Json::Value to_json()
		{
			nlohmann::json json = *this;
			Json::Value data;
			Json::Reader reader;

			reader.parse(json.dump(), data);

			return data;
		}

		NLOHMANN_DEFINE_TYPE_INTRUSIVE(PerformanceResponse, total_core, cpu_usage, memory_usage)
	};
}