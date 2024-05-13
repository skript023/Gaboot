#include <pch.h>

namespace gaboot
{
	struct AuthResponse
	{
		std::string success;
		std::string message;
		std::string token;

		void clear()
		{
			success.clear();
			message.clear();
			token.clear();
		}

		bool empty()
		{
			return success.empty() && message.empty() && token.empty();
		}

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