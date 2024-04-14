#pragma once
#include <pch.h>

#include "fwddec.hpp"

namespace gaboot
{
	struct response_data
	{
        Json::Value m_data;
        std::string m_message;
        bool m_success;
        int m_last_page;
        int m_total_data;

        Json::Value to_json() 
        {
            Json::Value json;
            if (!m_message.empty()) json["message"] = m_message;
            if (!m_data.empty()) json["data"] = m_data;
            if (m_last_page != 0) json["lastPage"] = m_last_page;
            if (m_total_data != 0) json["totalData"] = m_total_data;
            json["success"] = m_success;

            this->clear();

            return json;
        }

        void clear()
        {
            m_message.clear();
            m_data.clear();
            m_success = false;
            m_last_page = 0;
            m_last_page = 0;
        }

        template<typename T>
        inline void operator=(T&& args) { m_data = args.to_json(); }
        template<typename T>
        inline void operator<<(T&& args) { m_data.append(args.to_json()); }
	};
}