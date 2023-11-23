#include "tasks.hpp"
#include "hardware/processor.hpp"

namespace gaboot
{
	tasks::tasks()
	{
		//g_schedule->task([] { LOG(INFO) << "Schedule running test"; })->every(10s)->call();
		g_schedule->task([] { LOG(INFO) << "CPU Usage : " << g_processor->processor_utilization(); })->every(1s)->call();
	}
}