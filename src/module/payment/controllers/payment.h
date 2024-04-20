#pragma once

#include <drogon/HttpController.h>
#include <payment/services/payment_service.hpp>

using namespace drogon;

namespace gaboot
{
    class payment : public drogon::HttpController<payment>
    {
        payment_service m_payment_service;
    public:
        METHOD_LIST_BEGIN
        // use METHOD_ADD to add your custom processing function here;
        // METHOD_ADD(payment::get, "/{2}/{1}", Get); // path is /payment/{arg2}/{arg1}
        // METHOD_ADD(payment::your_method_name, "/{1}/{2}/list", Get); // path is /payment/{arg1}/{arg2}/list
        // ADD_METHOD_TO(payment::your_method_name, "/absolute/path/{1}/{2}/list", Get); // path is /absolute/path/{arg1}/{arg2}/list
        ADD_METHOD_TO(payment::create, "/payment", Post);
        ADD_METHOD_TO(payment::findOne, "/payment/{id}", Get);
        ADD_METHOD_TO(payment::callback, "/notification/", Post);
        ADD_METHOD_TO(payment::callback, "/notification", Post);
        METHOD_LIST_END
        // your declaration of processing function maybe like this:
        // void get(const HttpRequestPtr& req, std::function<void (const HttpResponsePtr &)> &&callback, int p1, std::string p2);
        // void your_method_name(const HttpRequestPtr& req, std::function<void (const HttpResponsePtr &)> &&callback, double p1, int p2) const;
        void create(HttpRequestPtr const& req, response_t&& callback);
        void findOne(HttpRequestPtr const& req, response_t&& callback, std::string&& id);
        void callback(HttpRequestPtr const& req, response_t&& callback);
    };
}
