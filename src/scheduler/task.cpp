#include "tasks.hpp"
#include "hardware/memory.hpp"
#include "hardware/processor.hpp"

namespace gaboot
{
	tasks::tasks()
	{
		//g_schedule->task([] { LOG(INFO) << "Schedule running test"; })->every(10s)->call();
	}
}	