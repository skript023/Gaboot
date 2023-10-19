#pragma once
#include "pch.hpp"

#include <drogon/HttpController.h>

#include "user/models/Users.h"

using namespace drogon;
using namespace orm;
using namespace drogon_model::gaboot;

namespace gaboot
{
	class user : public drogon::HttpController<user>
	{
		Mapper<Users> db()
		{
			return Mapper<Users>(DATABASE_CLIENT);
		}
	public:
		METHOD_LIST_BEGIN
		// use METHOD_ADD to add your custom processing function here;
		// METHOD_ADD(user::get, "/{2}/{1}", Get); // path is /user/{arg2}/{arg1}
		// METHOD_ADD(user::your_method_name, "/{1}/{2}/list", Get); // path is /user/{arg1}/{arg2}/list
		// ADD_METHOD_TO(user::your_method_name, "/absolute/path/{1}/{2}/list", Get); // path is /absolute/path/{arg1}/{arg2}/list
		ADD_METHOD_TO(user::findAll, "/user", Get);
		ADD_METHOD_TO(user::create, "/user", Post);
		ADD_METHOD_TO(user::findOne, "/user/{id}", Get);
		ADD_METHOD_TO(user::update, "/user/{id}", Patch);
		ADD_METHOD_TO(user::remove, "/user/{id}", Delete);
		METHOD_LIST_END
		// your declaration of processing function maybe like this:
		// void get(const HttpRequestPtr& req, std::function<void (const HttpResponsePtr &)> &&callback, int p1, std::string p2);
		// void your_method_name(const HttpRequestPtr& req, std::function<void (const HttpResponsePtr &)> &&callback, double p1, int p2) const;
		void findAll(HttpRequestPtr const&, response_t&&);
		void findOne(HttpRequestPtr const&, response_t&&, long long id);
		void create(HttpRequestPtr const&, response_t&&);
		void update(HttpRequestPtr const&, response_t&&, long long id);
		void remove(HttpRequestPtr const&, response_t&&, long long id);
	};
}
