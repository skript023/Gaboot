#pragma once

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
        explicit validator(nlohmann::json const& json);

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

        bool validate(Json::Value const& json, std::string& error);

        static void empty_string_check(Json::Value& jsonValue);

        static void reconstruct_json(Json::Value& jsonValue);

        nlohmann::json convert(const Json::Value& jsonCppValue)
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
        void insert_schema(std::map<std::string, std::string> validations);
        void insert_schema(nlohmann::json const& validations);
    private:
        nlohmann::json m_schema = {
            {"$schema", "http://json-schema.org/draft-07/schema#"},
            {"title", "Customer Schema"},
            {"type", "object"}
        };
    };
}