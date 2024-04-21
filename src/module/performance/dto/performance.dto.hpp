#pragma once
#include <pch.h>

#include "hardware/memory.hpp"
#include "hardware/processor.hpp"

namespace gaboot
{
	struct PerformanceResponse
	{
		int total_core;
		std::string cpu_usage;
		std::string memory_usage;

		bool empty()
		{
			return total_core == 0 && cpu_usage.empty() && memory_usage.empty();
		}

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