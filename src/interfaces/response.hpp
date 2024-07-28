#pragma once
#include <pch.h>

#include "fwddec.hpp"

namespace gaboot
{
    using namespace drogon;

    template<typename T>
	class response_data
	{
    public:
        std::string m_message;
        bool m_success;
        int m_last_page;
        int m_total_data;
        T m_data;

        Json::Value to_json() 
        {
            Json::Value json;

            if (!m_message.empty()) json["message"] = m_message;
            if (!m_data.empty()) json["data"] = m_data.to_json();;
            if (m_last_page != 0) json["lastPage"] = m_last_page;
            if (m_total_data != 0) json["totalData"] = m_total_data;
            json["success"] = m_success;

            this->clear();

            return json;
        }

        response_data* status(HttpStatusCode status)
        {
            this->m_status = status;

            return this;
        }

        HttpResponsePtr json()
        {
            HttpResponsePtr response = HttpResponse::newHttpJsonResponse(this->to_json());
            response->setStatusCode(m_status);

            return response;
        }

        void clear()
        {
            m_data.clear();
            m_message.clear();
            m_success = false;
            m_last_page = 0;
            m_last_page = 0;
        }

    private:
        HttpStatusCode m_status = k200OK;
	};
}