#pragma once

#include <drogon/HttpController.h>
#include "../services/banner_service.hpp"

using namespace drogon;

namespace gaboot
{
    class banner : public drogon::HttpController<banner>
    {
    public:
        METHOD_LIST_BEGIN
        // use METHOD_ADD to add your custom processing function here;
        // METHOD_ADD(banner::get, "/{2}/{1}", Get); // path is /banner/{arg2}/{arg1}
        // METHOD_ADD(banner::your_method_name, "/{1}/{2}/list", Get); // path is /banner/{arg1}/{arg2}/list
        // ADD_METHOD_TO(banner::your_method_name, "/absolute/path/{1}/{2}/list", Get); // path is /absolute/path/{arg1}/{arg2}/list
        ADD_METHOD_TO(banner::findAll, "/banner", Get);
        ADD_METHOD_TO(banner::findOne, "/banner/{id}", Get);
        //ADD_METHOD_TO(banner::update, "/banner/{id}", Put);
        //ADD_METHOD_TO(banner::remove, "/banner/{id}", Delete);
        ADD_METHOD_TO(banner::getImage, "/banner/image/{id}", Get);
        ADD_METHOD_TO(banner::getThumbnail, "/banner/thumbnail/{id}", Get);

        METHOD_LIST_END
        // your declaration of processing function maybe like this:
        // void get(const HttpRequestPtr& req, std::function<void (const HttpResponsePtr &)> &&callback, int p1, std::string p2);
        // void your_method_name(const HttpRequestPtr& req, std::function<void (const HttpResponsePtr &)> &&callback, double p1, int p2) const;
        void findAll(HttpRequestPtr const& req, response_t&& callback);
        void findOne(HttpRequestPtr const& req, response_t&& callback, std::string&& id);
        void getImage(HttpRequestPtr const& req, response_t&& callback, std::string&& id);
        void getThumbnail(HttpRequestPtr const& req, response_t&& callback, std::string&& id);
    private:
        banner_service m_banner_service;
    };
}
