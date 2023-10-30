#pragma once

#include <pch.h>
#include "schedule.hpp"

namespace gaboot
{
	class tasks
	{
	public:
		explicit tasks()
		{
			//g_schedule->task([] { LOG(INFO) << "Schedule running test"; })->every(10s)->call();
		}

		virtual ~tasks() = default;
	};
}