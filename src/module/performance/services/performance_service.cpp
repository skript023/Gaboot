#include "performance_service.hpp"

#include "hardware/memory.hpp"
#include "hardware/processor.hpp"

namespace gaboot
{
	HttpResponsePtr performance_service::get_server_status(HttpRequestPtr const& req)
	{
		try
		{
			Json::Value json;
			json["Logical CPU"] = std::thread::hardware_concurrency();
			json["CPU Usage"] = g_processor->processor_utilization();
			json["Memory Usage"] = g_virtual_memory->memory_used();

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