#pragma once

#include <drogon/HttpController.h>
#include "../services/category_service.hpp"

using namespace drogon;

namespace gaboot
{
    class category : public drogon::HttpController<category>
    {
    public:
        METHOD_LIST_BEGIN
        // use METHOD_ADD to add your custom processing function here;
        // METHOD_ADD(category::get, "/{2}/{1}", Get); // path is /category/{arg2}/{arg1}
        // METHOD_ADD(category::your_method_name, "/{1}/{2}/list", Get); // path is /category/{arg1}/{arg2}/list
        // ADD_METHOD_TO(category::your_method_name, "/absolute/path/{1}/{2}/list", Get); // path is /absolute/path/{arg1}/{arg2}/list
        //ADD_METHOD_TO(category::create, "/category", Post);
        ADD_METHOD_TO(category::findAll, "/category", Get);
        ADD_METHOD_TO(category::findOne, "/category/{id}", Get);
        //ADD_METHOD_TO(category::update, "/category/{id}", Put);
        //ADD_METHOD_TO(category::remove, "/category/{id}", Delete);
        ADD_METHOD_TO(category::getImage, "/category/image/{id}", Get);
        ADD_METHOD_TO(category::getThumbnail, "/category/thumbnail/{id}", Get);
        METHOD_LIST_END
        // your declaration of processing function maybe like this:
        // void get(const HttpRequestPtr& req, std::function<void (const HttpResponsePtr &)> &&callback, int p1, std::string p2);
        // void your_method_name(const HttpRequestPtr& req, std::function<void (const HttpResponsePtr &)> &&callback, double p1, int p2) const;
        void create(HttpRequestPtr const& req, response_t&& callback);
        void findAll(HttpRequestPtr const& req, response_t&& callback);
        void findOne(HttpRequestPtr const& req, response_t&& callback, std::string&& id);
        void update(HttpRequestPtr const& req, response_t&& callback, std::string&& id);
        void remove(HttpRequestPtr const& req, response_t&& callback, std::string&& id);
        void getImage(HttpRequestPtr const& req, response_t&& callback, std::string&& id);
        void getThumbnail(HttpRequestPtr const& req, response_t&& callback, std::string&& id);
    private:
        category_service m_category_service;
    };
}
