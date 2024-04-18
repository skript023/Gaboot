#include <pch.h>

namespace gaboot
{
	struct AuthResponse
	{
		std::string success;
		std::string message;
		std::string token;

		Json::Value to_json()
		{
			nlohmann::json json = *this;
			Json::Value data;
			Json::Reader reader;

			reader.parse(json.dump(), data);

			return data;
		}

		NLOHMANN_DEFINE_TYPE_INTRUSIVE(AuthResponse, success, message, token)
	};
}