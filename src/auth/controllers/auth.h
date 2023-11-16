#pragma once
#include "pch.h"
#include "interfaces/response.hpp"

#include <drogon/HttpController.h>
#include "auth/services/auth_service.hpp"

using namespace drogon;

namespace gaboot
{
	class auth : public HttpController<auth>
	{
		auth_service m_auth_service;
	public:
		METHOD_LIST_BEGIN
		// use METHOD_ADD to add your custom processing function here;
		// METHOD_ADD(auth::get, "/{2}/{1}", Get); // path is /gaboot/auth/{arg2}/{arg1}
		// METHOD_ADD(auth::your_method_name, "/{1}/{2}/list", Get); // path is /gaboot/auth/{arg1}/{arg2}/list
		// ADD_METHOD_TO(auth::your_method_name, "/absolute/path/{1}/{2}/list", Get); // path is /absolute/path/{arg1}/{arg2}/list
		ADD_METHOD_TO(auth::login, "/auth/login", Post);
		ADD_METHOD_TO(auth::logout, "/auth/logout", Get, CUSTOMER_MIDDLEWARE);
		METHOD_LIST_END
		// your declaration of processing function maybe like this:
		// void get(const HttpRequestPtr& req, std::function<void (const HttpResponsePtr &)> &&callback, int p1, std::string p2);
		// void your_method_name(const HttpRequestPtr& req, std::function<void (const HttpResponsePtr &)> &&callback, double p1, int p2) const;
		void login(HttpRequestPtr const& req, response_t&& callback);
		void logout(HttpRequestPtr const& req, response_t&& callback);
	};
}
