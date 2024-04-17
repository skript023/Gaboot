#pragma once
#include <pch.h>
#include "interfaces/response.hpp"

#include <drogon/HttpController.h>
#include "module/customer/models/MasterCustomers.h"
#include <auth/dto/auth.dto.hpp>

using namespace drogon;
using namespace orm;
using namespace drogon_model::gaboot;

namespace gaboot
{
	class auth_service
	{
		Mapper<MasterCustomers>db()
		{
			return Mapper<MasterCustomers>(DATABASE_CLIENT);
		}

		response_data<AuthResponse> m_response;
	public:
		HttpResponsePtr login(HttpRequestPtr const& req);
		HttpResponsePtr basic_login(HttpRequestPtr const& req);
		HttpResponsePtr basic_logout(HttpRequestPtr const& req);
		HttpResponsePtr logout(HttpRequestPtr const& req);
	};
}