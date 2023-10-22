#pragma once

#include <pch.h>
#include <util/gaboot.hpp>
#include <nlohmann/json.hpp>

namespace gaboot
{
    using nlohmann::json_schema::json_validator;

    struct validator_params
    {
        std::string m_field;
        std::string m_validation;
    };

    class validate_error : public nlohmann::json_schema::basic_error_handler
    {
        void error(const nlohmann::json_pointer<std::string>& pointer, const nlohmann::json& instance,
            const std::string& message) override
        {
            nlohmann::json_schema::basic_error_handler::error(pointer, instance, message);
            m_message = message;
            m_instance = instance;
            m_pointer = pointer;
        }

        std::string m_message;
        nlohmann::json m_instance;
        nlohmann::json_pointer<std::string> m_pointer;
    public:
        [[nodiscard]] std::string message() { return m_message; }
        [[nodiscard]] nlohmann::json instance() { return m_instance; }
        [[nodiscard]] nlohmann::json_pointer<std::string> pointer() { return m_pointer; }
    };

    class validator
    {
        json_validator m_validate; // create validator
    public:
        explicit validator(nlohmann::json const& json)
        {
            try
            {
                m_validate.set_root_schema(json); // insert root-schema
            }
            catch (std::exception const& e)
            {
                LOG_WARN << "Error : " << e.what();
            }
        }

        template<typename... Args>
        explicit validator(Args... args)
        {
            try
            {
                std::map<std::string, std::string> data;
                // Use a fold expression to iterate through the key-value pairs
                (..., (data[args.m_field] = args.m_validation));
                // Insert data into schema for validation
                this->insert_schema(data);
                // insert root-schema
                m_validate.set_root_schema(m_schema);
            }
            catch (std::exception const& e)
            {
                LOG_WARN << "Error : " << e.what();
            }
        }

        virtual ~validator() noexcept = default;

        bool validate(Json::Value const& json, std::string& error)
        {
            auto data = convert(json);

            validate_error err;
            auto res = m_validate.validate(data, err);

            if (err)
            {
                error += err.message() + " at " + err.pointer().to_string().substr(1);

                return false;
            }

            return true;
        }

        static void empty_string_check(Json::Value& jsonValue)
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

        static void reconstruct_json(Json::Value& jsonValue)
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

        inline nlohmann::json convert(const Json::Value& jsonCppValue)
        {
            nlohmann::json result;

            if (jsonCppValue.isObject()) {
                for (const auto& member : jsonCppValue.getMemberNames())
                {
                    result[member] = convert(jsonCppValue[member]);
                }
            }
            else if (jsonCppValue.isArray())
            {
                for (const auto& element : jsonCppValue)
                {
                    result.push_back(convert(element));
                }
            }
            else if (jsonCppValue.isString())
            {
                result = jsonCppValue.asString();
            }
            else if (jsonCppValue.isInt())
            {
                result = jsonCppValue.asInt();
            }
            else if (jsonCppValue.isDouble())
            {
                result = jsonCppValue.asDouble();
            }
            else if (jsonCppValue.isBool())
            {
                result = jsonCppValue.asBool();
            }
            else if (jsonCppValue.isNull())
            {
                result = nullptr;
            }

            return result;
        }

        void operator() (nlohmann::json const& json)
        {
            try
            {
                m_validate.set_root_schema(json); // insert root-schema
            }
            catch (std::exception const& e)
            {
                LOG_WARN << "Error : " << e.what();
            }
        }

        [[nodiscard]] nlohmann::json get_schema() { return m_schema; }
    private:
        void insert_schema(std::map<std::string, std::string> validations)
        {
            nlohmann::json properties;

            if (!validations.empty())
            {
                for (auto& validation : validations)
                {
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


    private:
        nlohmann::json m_schema = {
            {"$schema", "http://json-schema.org/draft-07/schema#"},
            {"title", "Customer Schema"},
            {"type", "object"}
        };
    };
}