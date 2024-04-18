#pragma once

namespace gaboot
{
    using traits = jwt::traits::nlohmann_json;
    class jwt_generator
    {
    public:
        jwt_generator() = default;
        virtual ~jwt_generator() noexcept = default;

        jwt_generator(nlohmann::json json)
        {
            auto hour = std::chrono::system_clock::now() + std::chrono::hours(1);

            m_token = jwt::create<traits>().
                set_expires_at(hour).
                set_issuer("gaboot").
                set_issued_at(std::chrono::system_clock::now()).
                set_type("JWS").
                set_payload_claim("gaboot", jwt::basic_claim<traits>(json.dump())).
                sign(jwt::algorithm::hs256{ SECRET });

            jwt_generator::insert(m_token, json.dump());
        }

        std::string get_token()
        {
            return m_token;
        }

        AuthResponse result()
        {
            AuthResponse response;
            response.message = "Token successfully generated";
            response.token = m_token;
            response.success = true;

            return response;
        }

        static void log_token()
        {
            std::ranges::for_each(m_cached_token, [](std::pair<std::string, std::string> param) 
            {
                LOG(INFO) << param.first;
            });
        }

        static void insert(std::string const& token, std::string const& payload)
        {
            m_cached_token.insert({ token, payload });
        }

        static bool find(std::string const& token, std::pair<std::string, std::string>& result)
        {
            if (auto it = m_cached_token.find(token); it != m_cached_token.end())
            {
                result = { it->first, it->second };

                return true;
            }

            return false;
        }

        static std::pair<std::string, std::string> find(std::string const& token)
        {
            if (auto it = m_cached_token.find(token); it != m_cached_token.end())
            {
                return { it->first, it->second };
            }

            return {};
        }

        static bool remove(std::string const& token)
        {
            if (auto it = m_cached_token.find(token); it != m_cached_token.end())
            {
                m_cached_token.erase(it);

                return true;
            }

            return false;
        }

        static bool parse_token(std::string const& header, std::string& token)
        {
            if (auto it = header.find(m_prefix); it != std::string::npos)
            {
                token = header.substr(sizeof(m_prefix) - 1);

                return true;
            }

            LOG(WARNING) << "Invalid Bearer token string.";

            return false;
        }

        static std::string parse_token(std::string const& header)
        {
            if (auto it = header.find(m_prefix); it != std::string::npos)
            {
                return header.substr(sizeof(m_prefix) - 1);
            }

            LOG(WARNING) << "Invalid Bearer token string.";

            return {};
        }
    private:
        std::string m_token;
        inline static constexpr const char* m_prefix = "Bearer ";
        inline static std::map<std::string, std::string> m_cached_token;
    };
}