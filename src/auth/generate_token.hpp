#pragma once

namespace gaboot
{
    using traits = jwt::traits::nlohmann_json;
    class jwt_create
    {
    public:
        virtual ~jwt_create() noexcept = default;

        jwt_create(nlohmann::json json)
        {
            auto hour = std::chrono::system_clock::now() + std::chrono::hours(1);

            m_token = jwt::create<traits>().set_expires_at(hour).
                set_payload_claim("access", jwt::basic_claim<traits>(json.dump())).
                sign(jwt::algorithm::hs256{ SECRET });
        }

        std::string get_token()
        {
            return m_token;
        }
    private:
        std::string m_token;
    };
}