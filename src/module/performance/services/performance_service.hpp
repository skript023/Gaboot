#pragma once
#include <pch.h>

#include "interfaces/response.hpp"
#include <performance/dto/performance.dto.hpp>

using namespace drogon;

namespace gaboot
{
	class performance_service
	{
	public:
		performance_service() = default;
		~performance_service() noexcept = default;

		performance_service(performance_service const&) = delete;
		performance_service& operator=(performance_service const&) = delete;

		performance_service(performance_service&&) = delete;
		performance_service& operator=(performance_service&&) = delete;

		HttpResponsePtr get_server_status(HttpRequestPtr const& req);
	private:
		response_data<PerformanceResponse> m_response;
	};
}