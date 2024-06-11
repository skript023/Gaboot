#pragma once

#include <drogon/HttpController.h>
#include <cart/services/cart_service.hpp>

using namespace drogon;

namespace gaboot
{
    class cart : public drogon::HttpController<cart>
    {
        cart_service m_cart_service;
    public:
        METHOD_LIST_BEGIN
        // use METHOD_ADD to add your custom processing function here;
        // METHOD_ADD(cart::get, "/{2}/{1}", Get); // path is /cart/{arg2}/{arg1}
        // METHOD_ADD(cart::your_method_name, "/{1}/{2}/list", Get); // path is /cart/{arg1}/{arg2}/list
        // ADD_METHOD_TO(cart::your_method_name, "/absolute/path/{1}/{2}/list", Get); // path is /absolute/path/{arg1}/{arg2}/list
        ADD_METHOD_TO(cart::create, "/cart", Post);
        ADD_METHOD_TO(cart::findAll, "/cart", Get);
        ADD_METHOD_TO(cart::findOne, "/cart/{id}", Get);
        ADD_METHOD_TO(cart::update, "/cart/{id}", Patch);
        ADD_METHOD_TO(cart::remove, "/cart/{id}", Delete);
        METHOD_LIST_END
        // your declaration of processing function maybe like this:
        // void get(const HttpRequestPtr& req, std::function<void (const HttpResponsePtr &)> &&callback, int p1, std::string p2);
        // void your_method_name(const HttpRequestPtr& req, std::function<void (const HttpResponsePtr &)> &&callback, double p1, int p2) const;
        void create(HttpRequestPtr const& req, response_t&& callback);
        void findAll(HttpRequestPtr const& req, response_t&& callback);
        void findOne(HttpRequestPtr const& req, response_t&& callback, std::string&& id);
        void update(HttpRequestPtr const& req, response_t&& callback, std::string&& id);
        void remove(HttpRequestPtr const& req, response_t&& callback, std::string&& id);
    };
}
