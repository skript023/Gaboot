#include <pch.h>
#include "validator.hpp"

namespace gaboot
{
	validator::validator(nlohmann::json const& json)
    {
        try
        {
            this->insert_schema(json);
            m_validate.set_root_schema(m_schema); // insert root-schema
        }
        catch (std::exception const& e)
        {
            LOG(WARNING) << "Error : " << e.what();
        }
    }
    bool validator::validate(Json::Value const& json, std::string& error)
    {
        auto data = convert(json);

        validate_error err;
        auto res = m_validate.validate(data, err);

        if (err)
        {
            error += err.message() + " at " + err.pointer().to_string().substr(1);
            LOG(WARNING) << fmt::format("{} at {}", err.message(), err.pointer().to_string().substr(1));

            return false;
        }

        return true;
    }
    void validator::empty_string_check(Json::Value& jsonValue)
    {
        if (jsonValue.isObject())
        {
            for (Json::ValueIterator it = jsonValue.begin(); it != jsonValue.end(); ++it)
            {
                if ((*it).isString() && (*it).asString().empty())
                {
                    (*it) = Json::Value(Json::nullValue);
                }
                else
                {
                    empty_string_check(*it);
                }
            }
        }
        else if (jsonValue.isArray())
        {
            for (Json::ValueIterator it = jsonValue.begin(); it != jsonValue.end(); ++it)
            {
                empty_string_check(*it);
            }
        }
    }
    void validator::reconstruct_json(Json::Value& jsonValue)
    {
        if (jsonValue.isObject())
        {
            Json::Value::Members members = jsonValue.getMemberNames();
            for (const std::string& member : members)
            {
                if (jsonValue[member].isString() && jsonValue[member].asString().empty())
                {
                    jsonValue.removeMember(member);
                }
                else
                {
                    reconstruct_json(jsonValue[member]);
                }
            }
        }
        else if (jsonValue.isArray())
        {
            for (Json::ArrayIndex i = 0; i < jsonValue.size(); ++i)
            {
                reconstruct_json(jsonValue[i]);
            }
        }
    }
    void validator::insert_schema(std::map<std::string, std::string> validations)
    {
        nlohmann::json properties;

        if (!validations.empty())
        {
            for (auto& validation : validations)
            {
                validation.second.erase(std::remove_if(validation.second.begin(), validation.second.end(), ::isspace), validation.second.end());

                std::vector<std::string> components;
                size_t startPos = 0;
                size_t pipePos = validation.second.find('|');

                while (pipePos != std::string::npos)
                {
                    components.push_back(validation.second.substr(startPos, pipePos - startPos));
                    startPos = pipePos + 1;
                    pipePos = validation.second.find('|', startPos);
                }

                components.push_back(validation.second.substr(startPos)); // Add the last component

                // Process the components and extract key-value pairs
                std::map<std::string, std::string> extracted;

                for (const std::string& component : components)
                {
                    size_t colonPos = component.find(':');
                    if (colonPos != std::string::npos)
                    {
                        std::string key = component.substr(0, colonPos);
                        std::string value = component.substr(colonPos + 1);
                        extracted[key] = value;
                    }
                    else
                    {
                        // No key-value pair, treat the component as a key with an empty value
                        extracted[component] = "";
                    }
                }

                m_schema["properties"][validation.first]["description"] = validation.first;

                if (auto it = extracted.find("regex"); it != extracted.end())
                {
                    if (!it->second.empty())
                    {
                        m_schema["properties"][validation.first]["regex"] = it->second;
                    }
                }

                if (auto it = extracted.find("maxLength"); it != extracted.end())
                {
                    if (!it->second.empty() && util::is_numeric(it->second))
                    {
                        m_schema["properties"][validation.first]["maxLength"] = stoi(it->second);
                    }
                }

                if (auto it = extracted.find("minLength"); it != extracted.end())
                {
                    if (!it->second.empty() && util::is_numeric(it->second))
                    {
                        m_schema["properties"][validation.first]["minLength"] = stoi(it->second);
                    }
                }

                if (auto it = extracted.find("type"); it != extracted.end())
                {
                    if (!it->second.empty())
                    {
                        m_schema["properties"][validation.first]["type"] = it->second;
                    }
                }

                if (auto it = extracted.find("required"); it != extracted.end())
                {
                    m_schema["required"].push_back(validation.first);
                }

                if (auto it = extracted.find("alphabetOnly"); it != extracted.end())
                {
                    m_schema["properties"][validation.first]["pattern"] = "^[A-Za-z]+$";
                }

                if (auto it = extracted.find("numberOnly"); it != extracted.end())
                {
                    m_schema["properties"][validation.first]["pattern"] = "^[0-9]+$";
                }

                if (auto it = extracted.find("alphanum"); it != extracted.end())
                {
                    m_schema["properties"][validation.first]["pattern"] = "^[A-Za-z0-9]+$";
                }

                if (auto it = extracted.find("email"); it != extracted.end())
                {
                    m_schema["properties"][validation.first]["pattern"] = "^\\S+@\\S+\\.\\S+$";
                }
            }
        }
    }
    void validator::insert_schema(nlohmann::json const& validations)
    {
        if (!validations.empty())
        {
            for (auto& validation : validations.items())
            {
                //validation.second.erase(std::remove_if(validation.second.begin(), validation.second.end(), ::isspace), validation.second.end());

                std::vector<std::string> components;
                size_t startPos = 0;
                size_t pipePos = validation.value().get<std::string>().find('|');

                while (pipePos != std::string::npos)
                {
                    components.push_back(validation.value().get<std::string>().substr(startPos, pipePos - startPos));
                    startPos = pipePos + 1;
                    pipePos = validation.value().get<std::string>().find('|', startPos);
                }

                components.push_back(validation.value().get<std::string>().substr(startPos)); // Add the last component

                // Process the components and extract key-value pairs
                std::map<std::string, std::string> extracted;

                for (const std::string& component : components)
                {
                    size_t colonPos = component.find(':');
                    if (colonPos != std::string::npos)
                    {
                        std::string key = component.substr(0, colonPos);
                        std::string value = component.substr(colonPos + 1);
                        extracted[key] = value;
                    }
                    else
                    {
                        // No key-value pair, treat the component as a key with an empty value
                        extracted[component] = "";
                    }
                }

                m_schema["properties"][validation.key()]["description"] = validation.key();

                if (auto it = extracted.find("regex"); it != extracted.end())
                {
                    if (!it->second.empty())
                    {
                        m_schema["properties"][validation.key()]["regex"] = it->second;
                    }
                }

                if (auto it = extracted.find("maxLength"); it != extracted.end())
                {
                    if (!it->second.empty() && util::is_numeric(it->second))
                    {
                        m_schema["properties"][validation.key()]["maxLength"] = stoi(it->second);
                    }
                }

                if (auto it = extracted.find("minLength"); it != extracted.end())
                {
                    if (!it->second.empty() && util::is_numeric(it->second))
                    {
                        m_schema["properties"][validation.key()]["minLength"] = stoi(it->second);
                    }
                }

                if (auto it = extracted.find("type"); it != extracted.end())
                {
                    if (!it->second.empty())
                    {
                        m_schema["properties"][validation.key()]["type"] = it->second;
                    }
                }

                if (auto it = extracted.find("required"); it != extracted.end())
                {
                    m_schema["required"].push_back(validation.key());
                }

                if (auto it = extracted.find("alphabetOnly"); it != extracted.end())
                {
                    m_schema["properties"][validation.key()]["pattern"] = "^[A-Za-z]+$";
                }

                if (auto it = extracted.find("numberOnly"); it != extracted.end())
                {
                    m_schema["properties"][validation.key()]["pattern"] = "^[0-9]+$";
                }

                if (auto it = extracted.find("alphanum"); it != extracted.end())
                {
                    m_schema["properties"][validation.key()]["pattern"] = "^[A-Za-z0-9]+$";
                }

                if (auto it = extracted.find("email"); it != extracted.end())
                {
                    m_schema["properties"][validation.key()]["pattern"] = "^\\S+@\\S+\\.\\S+$";
                }
            }
        }
    }
}