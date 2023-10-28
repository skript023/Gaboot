#pragma once
#define ADMIN_MIDDLEWARE_ACTIVE
#define CUSTOMER_MIDDLEWARE_ACTIVE

#include "pch.h"

#include <drogon/HttpController.h>

#include "util/validator.hpp"
#include "module/customer/services/customer_service.hpp"
#include "module/customer/models/MasterCustomers.h"

using namespace drogon;
using namespace orm;
using namespace drogon_model::gaboot;

namespace gaboot
{
	class customer : public drogon::HttpController<customer>
	{
		customer_service m_customer_service;
	public:
		METHOD_LIST_BEGIN
		// use METHOD_ADD to add your custom processing function here;
		// METHOD_ADD(user::get, "/{2}/{1}", Get); // path is /user/{arg2}/{arg1}
		// METHOD_ADD(user::your_method_name, "/{1}/{2}/list", Get); // path is /user/{arg1}/{arg2}/list
		// ADD_METHOD_TO(user::your_method_name, "/absolute/path/{1}/{2}/list", Get); // path is /absolute/path/{arg1}/{arg2}/list
		ADD_METHOD_TO(customer::findAll, "/customers", Get, ADMIN_MIDDLEWARE);
		ADD_METHOD_TO(customer::create, "/customers", Post);
		ADD_METHOD_TO(customer::findOne, "/customers/{id}", Get, CUSTOMER_MIDDLEWARE);
		ADD_METHOD_TO(customer::update, "/customers/{id}", Put, CUSTOMER_MIDDLEWARE);
		ADD_METHOD_TO(customer::remove, "/customers/{id}", Delete, ADMIN_MIDDLEWARE);
		ADD_METHOD_TO(customer::getImage, "/customers/image/{id}", Get, CUSTOMER_MIDDLEWARE);
		ADD_METHOD_TO(customer::getProfile, "/customers/profile/{id}", Get, CUSTOMER_MIDDLEWARE);
		METHOD_LIST_END
		// your declaration of processing function maybe like this:
		// void get(const HttpRequestPtr& req, std::function<void (const HttpResponsePtr &)> &&callback, int p1, std::string p2);
		// void your_method_name(const HttpRequestPtr& req, std::function<void (const HttpResponsePtr &)> &&callback, double p1, int p2) const;
		void findAll(HttpRequestPtr const&, response_t&&);
		void findOne(HttpRequestPtr const&, response_t&&, std::string&& id);
		void create(HttpRequestPtr const&, response_t&&);
		void update(HttpRequestPtr const&, response_t&&, std::string&& id);
		void remove(HttpRequestPtr const&, response_t&&, std::string&& id);
		void getImage(HttpRequestPtr const&, response_t&&, std::string&& id);
		void getProfile(HttpRequestPtr const&, response_t&&, std::string&& id);
	};
}
