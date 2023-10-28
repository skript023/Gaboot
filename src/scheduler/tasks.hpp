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
			drogon::app().getLoop()->runInLoop(tasks::run);
		}

		virtual ~tasks() = default;

		static void run()
		{
			g_schedule->task([] { LOG(INFO) << "Schedule running test"; })->every(10s);
			g_schedule->task([] { LOG(INFO) << "Schedule running test 2"; })->every(5s);
		}
	};
}