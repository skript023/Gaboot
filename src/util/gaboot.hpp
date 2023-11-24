#pragma once
#include <logger.hpp>

namespace gaboot::util
{
	inline std::string get_env(std::string const& key)
	{
        std::ifstream env_file(".env");
        if (!env_file.is_open())
        {
            std::cerr << "Unable to open .env file" << std::endl;
            return {};
        }

        std::map<std::string, std::string> variables;

        for (std::string line; std::getline(env_file, line);)
        {
            size_t found = line.find('=');
            if (found != std::string::npos)
            {
                std::string key = line.substr(0, found);
                std::string value = line.substr(found + 1);
                variables[key] = value;
            }
        }

        env_file.close();

        return variables[key];
	}

    inline bool is_numeric(std::string const& str)
    {
        std::regex numeric("^[0-9]+$");

        return std::regex_match(str, numeric);
    }
    
    inline bool is_numeric(std::string_view const& str)
    {
        std::regex numeric("^[0-9]+$");

        return std::regex_match(str.data(), numeric);
    }

    inline bool is_phone_number(std::string const& phoneNumber)
    {
        std::regex pattern("^(\\+\\d{1,3})?\\s?\\(?(\\d{3})\\)?[-.\\s]?(\\d{3})[-.\\s]?(\\d{4})$");

        return std::regex_match(phoneNumber, pattern);
    }

    inline bool allowed_image(std::string_view const& str)
    {
        std::regex extension(".*\\.(jpg|jpeg|png|gif)$");

        return std::regex_match(str.begin(), str.end(), extension);
    }
    
    inline bool allowed_image(std::string const& str)
    {
        std::regex extension(".*\\.(jpg|jpeg|png|gif)$");

        return std::regex_match(str, extension);
    }
    
    inline bool allowed_image(const char* str)
    {
        std::vector<std::string> extensions = { "jpg", "jpeg", "png", "gif" };

        for (auto& extension : extensions)
        {
            if (strcmp(extension.c_str(), str) == 0)
            {
                return true;
            }
        }

        return false;
    }

    inline bool multipart_tojson(const drogon::MultiPartParser& multipart, Json::Value& json)
    {
        if (const auto& params = multipart.getParameters(); !params.empty())
        {
            for (const auto& param : params)
            {
                if (param.first == "password")
                {
                    json[param.first] = bcrypt::generateHash(param.second);
                }
                else
                {
                    json[param.first] = param.second;
                }
            }

            return true;
        }

        LOG(WARNING) << "Failed get parameters because size is " << multipart.getParameters().size();

        return false;
    }
}