#pragma once

#include <pch.h>
#include <util/gaboot.hpp>

namespace gaboot
{
    using nlohmann::json_schema::json_validator;

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

        ~validator() noexcept = default;

        bool validate(Json::Value const& json, std::string& error)
        {
            auto data = convert(json);

            try
            {
                auto res = m_validate.validate(data);
                LOG_INFO << res.dump();

                return true;
            }
            catch (std::exception const& e)
            {
                error += e.what();
                LOG_WARN << "Error : " << e.what();

                return false;
            }

            return false;
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
    };
}