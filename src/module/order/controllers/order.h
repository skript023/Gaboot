#pragma once
#include "pch.h"

#include <drogon/HttpController.h>
#include "order/services/order_service.hpp"

using namespace drogon;

namespace gaboot
{
	class order : public drogon::HttpController<order>
	{
		order_service m_order_service;
	public:
		METHOD_LIST_BEGIN
		// use METHOD_ADD to add your custom processing function here;
		// METHOD_ADD(order::get, "/{2}/{1}", Get); // path is /order/{arg2}/{arg1}
		// METHOD_ADD(order::your_method_name, "/{1}/{2}/list", Get); // path is /order/{arg1}/{arg2}/list
		// ADD_METHOD_TO(order::your_method_name, "/absolute/path/{1}/{2}/list", Get); // path is /absolute/path/{arg1}/{arg2}/list
		ADD_METHOD_TO(order::create, "/order", Post, CUSTOMER_MIDDLEWARE);
		ADD_METHOD_TO(order::findAll, "/order", Get, CUSTOMER_MIDDLEWARE);
		ADD_METHOD_TO(order::findOne, "/order/{id}", Get, CUSTOMER_MIDDLEWARE);
		ADD_METHOD_TO(order::update, "/order/{id}", Patch, CUSTOMER_MIDDLEWARE);
		METHOD_LIST_END
		// your declaration of processing function maybe like this:
		// void get(const HttpRequestPtr& req, std::function<void (const HttpResponsePtr &)> &&callback, int p1, std::string p2);
		// void your_method_name(const HttpRequestPtr& req, std::function<void (const HttpResponsePtr &)> &&callback, double p1, int p2) const;
		void create(HttpRequestPtr const& req, response_t&& callback);
		void findAll(HttpRequestPtr const& req, response_t&& callback);
		void findOne(HttpRequestPtr const& req, response_t&& callback, std::string&& id);
		void update(HttpRequestPtr const& req, response_t&& callback, std::string&& id);
	};
}