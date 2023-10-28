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
#include <opencv2/opencv.hpp>

#include <jwt-cpp/traits/nlohmann-json/traits.h>
#pragma warning(push)
#pragma warning(disable:4996)
#include <bcrypt.h>
#pragma warning(pop)

#include <util/gaboot.hpp>
#include <util/logger.hpp>
#include <util/validator.hpp>

//drogon::app().getLoop()->runEvery(std::chrono::milliseconds(1), [] { LOG(INFO) << "Looped"; });

#define NUM_THREAD drogon::app().getThreadNum()
#define ADD_LISTENER(IP, PORT) drogon::app().addListener(IP, PORT)
#define ADD_CONFIG(JSON) drogon::app().loadConfigFile(JSON)
#define SET_UPLOADED_PATH(path) drogon::app().setUploadPath(path);
#define SERVER_RUN drogon::app().run()
#define UPLOAD_PATH drogon::app().getUploadPath()
#define DATABASE_CLIENT drogon::app().getDbClient()

#define SECRET util::get_env("SECRET")
#define SERVER_KEY util::get_env("SERVER_KEY")

#define ADMIN_MIDDLEWARE_DISABLE

#ifdef ADMIN_MIDDLEWARE_DISABLE
#define ADMIN_MIDDLEWARE
#define ADMIN_MIDDLEWARE_STATUS "Deactivated"
#else
#define ADMIN_MIDDLEWARE "gaboot::admin_middleware"
#define ADMIN_MIDDLEWARE_STATUS "Activated"
#endif

#ifdef CUSTOMER_MIDDLEWARE_DISABLE
#define CUSTOMER_MIDDLEWARE
#define CUSTOMER_MIDDLEWARE_STATUS "Deactivated"
#else
#define CUSTOMER_MIDDLEWARE "gaboot::customer_middleware"
#define CUSTOMER_MIDDLEWARE_STATUS "Activated"
#endif // CUSTOMER_MIDDLEWARE_ACTIVE

#ifdef _WIN32
#define OS_NAME "Windows"
#elif __linux__
#define OS_NAME "Linux"
#else
#define OS_NAME "Unknown"
#endif

using response_t = std::function<void(drogon::HttpResponsePtr const&)>;
using Multer = drogon::HttpFile;

using namespace std::chrono_literals;