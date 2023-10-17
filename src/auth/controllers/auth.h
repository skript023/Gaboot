#pragma once

#include <drogon/HttpController.h>
#include "user/models/Users.h"

using namespace drogon;
using namespace orm;
using namespace drogon_model::gaboot;

namespace gaboot
{
	class auth : public HttpController<auth>
	{
		Mapper<Users>db()
		{
			return Mapper<Users>(DATABASE_CLIENT);
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
		void payment(HttpRequestPtr const& req, response_t&& callback)
		{
			std::string order_id;

			nlohmann::json json = {
				{
					"transaction_detail", {
						{"order_id", order_id}
					}
				}
			};
			
			cpr::Url url = "https://app.sandbox.midtrans.com/snap/v1/transactions";

			auto res = cpr::Post(url, cpr::Body(json.dump()));
		}
	};
}
