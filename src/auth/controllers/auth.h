#pragma once
#include "pch.h"

#include <drogon/HttpController.h>
#include "module/customer/models/MasterCustomers.h"

using namespace drogon;
using namespace orm;
using namespace drogon_model::gaboot;

namespace gaboot
{
	class auth : public HttpController<auth>
	{
		Mapper<MasterCustomers>db()
		{
			return Mapper<MasterCustomers>(DATABASE_CLIENT);
		}
	public:
		METHOD_LIST_BEGIN
		// use METHOD_ADD to add your custom processing function here;
		// METHOD_ADD(auth::get, "/{2}/{1}", Get); // path is /gaboot/auth/{arg2}/{arg1}
		// METHOD_ADD(auth::your_method_name, "/{1}/{2}/list", Get); // path is /gaboot/auth/{arg1}/{arg2}/list
		// ADD_METHOD_TO(auth::your_method_name, "/absolute/path/{1}/{2}/list", Get); // path is /absolute/path/{arg1}/{arg2}/list
		ADD_METHOD_TO(auth::login, "/auth/login", Post);
		METHOD_LIST_END
		// your declaration of processing function maybe like this:
		// void get(const HttpRequestPtr& req, std::function<void (const HttpResponsePtr &)> &&callback, int p1, std::string p2);
		// void your_method_name(const HttpRequestPtr& req, std::function<void (const HttpResponsePtr &)> &&callback, double p1, int p2) const;
		void login(HttpRequestPtr const& req, response_t&& callback);
	};
}
