#pragma once
#include <pch.h>

#include <drogon/HttpController.h>
#include <performance/services/performance_service.hpp>

using namespace drogon;

namespace gaboot
{
    class performance : public drogon::HttpController<performance>
    {
        performance_service m_performance_service;
    public:
        METHOD_LIST_BEGIN
        // use METHOD_ADD to add your custom processing function here;
        // METHOD_ADD(performance::get, "/{2}/{1}", Get); // path is /performance/{arg2}/{arg1}
        // METHOD_ADD(performance::your_method_name, "/{1}/{2}/list", Get); // path is /performance/{arg1}/{arg2}/list
        // ADD_METHOD_TO(performance::your_method_name, "/absolute/path/{1}/{2}/list", Get); // path is /absolute/path/{arg1}/{arg2}/list
        ADD_METHOD_TO(performance::get_server_status, "/server/status", Get);
        METHOD_LIST_END
        // your declaration of processing function maybe like this:
        // void get(const HttpRequestPtr& req, std::function<void (const HttpResponsePtr &)> &&callback, int p1, std::string p2);
        // void your_method_name(const HttpRequestPtr& req, std::function<void (const HttpResponsePtr &)> &&callback, double p1, int p2) const;
        void get_server_status(HttpRequestPtr const& req, response_t&& callback);
    };

}