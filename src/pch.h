#pragma once

#define JWT_DISABLE_PICOJSON

#include <iostream>
#include <string>
#include <thread>
#include <stack>
#include <regex>
#include <array>
#include <chrono>
#include <fstream>
#include <optional>
#include <exception>
#include <functional>
#include <filesystem>
#include <algorithm>
#include <cstdio>
#include <cstdint>

#ifdef __GNUC__

#elif _MSC_VER
	#include <Windows.h>
#endif

#include <fmt/format.h>
#include <drogon/drogon.h>
#include <nlohmann/json.hpp>
#include <nlohmann/json-schema.hpp>
#include <jwt-cpp/jwt.h>
#include <cpr/cpr.h>

#include <jwt-cpp/traits/nlohmann-json/traits.h>
#pragma warning(push)
#pragma warning(disable:4996)
#include <bcrypt.h>
#pragma warning(pop)

#include <util/gaboot.hpp>
#include <util/logger.hpp>
#include <util/validator.hpp>

#define ADD_LISTENER(IP, PORT) drogon::app().addListener(IP, PORT)
#define ADD_CONFIG(JSON) drogon::app().loadConfigFile(JSON)
#define SERVER_RUN drogon::app().run()
#define UPLOAD_PATH drogon::app().getUploadPath()
#define DATABASE_CLIENT drogon::app().getDbClient()

#ifdef _WIN32
#define OS_NAME "Windows"
#elif __linux__
#define OS_NAME "Linux"
#else
#define OS_NAME "Unknown"
#endif


#define SECRET "BahasaAsu123"
#define SERVER_KEY "U0ItTWlkLXNlcnZlci1GTzllNFFRTlZjVVJmUEYtb2UxMWU5ZFg="

using response_t = std::function<void(drogon::HttpResponsePtr const&)>;

using namespace std::chrono_literals;