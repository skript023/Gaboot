#pragma once

namespace gaboot
{
    class jwt_create
    {
    public:
        virtual ~jwt_create() noexcept = default;

        jwt_create(nlohmann::json json)
        {
            auto hour = std::chrono::system_clock::now() + std::chrono::hours(1);
            std::string payload = json.dump();

            m_token = jwt::create().
                set_expires_at(hour).
                set_payload_claim("access", jwt::claim(payload.begin(), payload.end())).
                sign(jwt::algorithm::hs256{ SECRET });
        }

        jwt::traits::kazuho_picojson::string_type get_token()
        {
            return m_token;
        }
    private:
        jwt::traits::kazuho_picojson::string_type m_token;
    };
}