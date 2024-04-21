#pragma once
#include <pch.h>

#include "fwddec.hpp"

namespace gaboot
{
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

        void clear()
        {
            m_message.clear();
            m_success = false;
            m_last_page = 0;
            m_last_page = 0;
        }
	};
}