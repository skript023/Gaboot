#include "tasks.hpp"
#include "hardware/memory.hpp"
#include "hardware/processor.hpp"

namespace gaboot
{
	tasks::tasks()
	{
		g_schedule->task([] { g_logger->reset(); })->every(24h)->call();
	}
}	