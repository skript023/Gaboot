#include "performance.h"

// Add definition of your processing function here
namespace gaboot
{
	void performance::get_server_status(HttpRequestPtr const& req, response_t&& callback)
	{
		return callback(m_performance_service.get_server_status(req));
	}
}