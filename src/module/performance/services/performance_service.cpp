#include "performance_service.hpp"

#include "hardware/memory.hpp"
#include "hardware/processor.hpp"

namespace gaboot
{
	HttpResponsePtr performance_service::get_server_status(HttpRequestPtr const& req)
	{
		try
		{
			PerformanceResponse json;

			json.total_core = std::thread::hardware_concurrency();
			json.memory_usage = std::thread::hardware_concurrency();
			json.cpu_usage = g_processor->processor_utilization();

			m_response.m_message = "Success get server status";
			m_response.m_success = true;
			m_response.m_data = json;

			return HttpResponse::newHttpJsonResponse(m_response.to_json());
		}
		catch (const std::exception& e)
		{
			LOG(WARNING) << e.what();

			m_response.m_message = fmt::format("Error caught : {}", e.what());
			m_response.m_success = false;

			auto response = HttpResponse::newHttpJsonResponse(m_response.to_json());
			response->setStatusCode(k500InternalServerError);

			return response;
		}
	}
}