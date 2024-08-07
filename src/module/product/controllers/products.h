#pragma once

#include <pch.h>
#include <drogon/HttpController.h>
#include <module/product/services/product_service.hpp>

using namespace drogon;

namespace gaboot
{
    class products : public drogon::HttpController<products>
    {
        product_service m_product_service;
    public:
        METHOD_LIST_BEGIN
        // use METHOD_ADD to add your custom processing function here;
        // METHOD_ADD(products::get, "/{2}/{1}", Get); // path is /products/{arg2}/{arg1}
        // METHOD_ADD(products::your_method_name, "/{1}/{2}/list", Get); // path is /products/{arg1}/{arg2}/list
        // ADD_METHOD_TO(products::your_method_name, "/absolute/path/{1}/{2}/list", Get); // path is /absolute/path/{arg1}/{arg2}/list
        //ADD_METHOD_TO(products::create, "/products", Post, ADMIN_MIDDLEWARE);
        ADD_METHOD_TO(products::findAll, "/products", Get);
        ADD_METHOD_TO(products::findOne, "/products/{id}", Get);
        //ADD_METHOD_TO(products::update, "/products/{id}", Put, ADMIN_MIDDLEWARE);
        //ADD_METHOD_TO(products::remove, "/products/{id}", Delete, ADMIN_MIDDLEWARE);
        ADD_METHOD_TO(products::productWithImages, "/products/product-image", Get);
        METHOD_LIST_END
        // your declaration of processing function maybe like this:
        // void get(const HttpRequestPtr& req, std::function<void (const HttpResponsePtr &)> &&callback, int p1, std::string p2);
        // void your_method_name(const HttpRequestPtr& req, std::function<void (const HttpResponsePtr &)> &&callback, double p1, int p2) const;
        void create(HttpRequestPtr const&, response_t&&);
        void findAll(HttpRequestPtr const&, response_t&&);
        void findOne(HttpRequestPtr const&, response_t&&, std::string&& id);
        void update(HttpRequestPtr const&, response_t&&, std::string&& id);
        void remove(HttpRequestPtr const&, response_t&&, std::string&& id);
        void productWithImages(HttpRequestPtr const&, response_t&&);
    };
}