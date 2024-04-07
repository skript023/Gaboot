#pragma once
#include <logger.hpp>
#include <chrono>

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
                json[param.first] = param.second;
            }

            return true;
        }

        LOG(WARNING) << "Failed get parameters because size is " << multipart.getParameters().size();

        return false;
    }

    inline Json::Value to_jsoncpp(const nlohmann::json& nlohmannJson)
    {
        Json::Value cppJson;

        if (nlohmannJson.is_object()) {
            for (auto it = nlohmannJson.begin(); it != nlohmannJson.end(); ++it) 
            {
                const auto& key = it.key();
                const auto& value = it.value();

                if (value.is_null())
                {
                    cppJson[key] = Json::nullValue;
                }
                else if (value.is_boolean())
                {
                    cppJson[key] = value.get<bool>();
                }
                else if (value.is_number_integer()) 
                {
                    cppJson[key] = value.get<int>();
                }
                else if (value.is_number_float())
                {
                    cppJson[key] = value.get<double>();
                }
                else if (value.is_string()) 
                {
                    cppJson[key] = value.get<std::string>();
                }
                else if (value.is_array()) 
                {
                    Json::Value array(Json::arrayValue);
                    for (const auto& element : value)
                    {
                        array.append(to_jsoncpp(element));
                    }
                    cppJson[key] = array;
                }
                else if (value.is_object())
                {
                    cppJson[key] = to_jsoncpp(value);
                }
            }
        }
        else if (nlohmannJson.is_array())
        {
            for (size_t i = 0; i < nlohmannJson.size(); ++i) 
            {
                cppJson.append(to_jsoncpp(nlohmannJson[i]));
            }
        }

        return cppJson;
    }

    inline nlohmann::json to_nlohmann_json(const Json::Value& cppJson) 
    {
        nlohmann::json nlohmannJson;

        if (cppJson.isObject()) {
            for (const auto& key : cppJson.getMemberNames()) 
            {
                const auto& value = cppJson[key];

                if (value.isNull())
                {
                    nlohmannJson[key] = nullptr;
                }
                else if (value.isBool()) 
                {
                    nlohmannJson[key] = value.asBool();
                }
                else if (value.isInt())
                {
                    nlohmannJson[key] = value.asInt();
                }
                else if (value.isDouble()) 
                {
                    nlohmannJson[key] = value.asDouble();
                }
                else if (value.isString()) 
                {
                    nlohmannJson[key] = value.asString();
                }
                else if (value.isArray()) 
                {
                    nlohmann::json array;
                    for (int i = 0; i < value.size(); ++i) 
                    {
                        array.push_back(to_nlohmann_json(value[i]));
                    }
                    nlohmannJson[key] = array;
                }
                else if (value.isObject())
                {
                    nlohmannJson[key] = to_nlohmann_json(value);
                }
            }
        }
        else if (cppJson.isArray()) 
        {
            for (int i = 0; i < cppJson.size(); ++i) 
            {
                nlohmannJson.push_back(to_nlohmann_json(cppJson[i]));
            }
        }

        return nlohmannJson;
    }

    inline bool is_json(std::string const& str)
    {
        try 
        {
            // Attempt to parse the string
            nlohmann::json::parse(str);
            // If parsing succeeds, it's valid JSON
            return true;
        }
        catch (nlohmann::json::parse_error& ex)
        {
            LOG(WARNING) << "The string is not json type, " << ex.what();
            return false;
        }
    }
#ifdef _WIN32
    inline std::string current_datetime()
    {
        auto const time = std::chrono::current_zone()->to_local(std::chrono::system_clock::now());
        return std::format("{:%Y-%m-%d %X}", time);
    }

    inline std::string current_datetime_plus_24_hours() {
        // Get the current time
        auto now = std::chrono::system_clock::now();

        // Add 24 hours to the current time
        auto in_24_hours = now + std::chrono::hours(24);

        // Convert to local time
        auto time = std::chrono::current_zone()->to_local(in_24_hours);

        // Format the time
        std::ostringstream oss;
        oss << std::format("{:%Y-%m-%d %X}", time);
        return oss.str();
    }
#endif
}