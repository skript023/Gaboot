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
#include <ranges>

#ifdef _WIN32
#include <Windows.h>
#include "psapi.h"
#include <Pdh.h>

#pragma comment(lib, "pdh.lib")
#elif __linux__
	
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

#include <gaboot.hpp>
#include <logger.hpp>
#include <validator/validator.hpp>

//drogon::app().getLoop()->runEvery(std::chrono::milliseconds(1), [] { LOG(INFO) << "Looped"; });

#define RUN_EVERY(duration, callback) drogon::app().getLoop()->runEvery(duration, callback)
#define NUM_THREAD drogon::app().getThreadNum()
#define ADD_LISTENER(IP, PORT) drogon::app().addListener(IP, PORT)
#define ADD_CONFIG(JSON) drogon::app().loadConfigFile(JSON)
#define SET_UPLOADED_PATH(path) drogon::app().setUploadPath(path);
#define SERVER_RUN drogon::app().run()
#define UPLOAD_PATH drogon::app().getUploadPath()
#define DATABASE_CLIENT drogon::app().getDbClient()

#define SECRET std::getenv("SECRET")
#define SERVER_KEY std::getenv("SERVER_KEY")

#ifdef BASIC_MIDDLEWARE_DISABLE
#define BASIC_MIDDLEWARE
#define BASIC_MIDDLEWARE_STATUS "Deactivated"
#else
#define BASIC_MIDDLEWARE "gaboot::basic_middleware"
#define BASIC_MIDDLEWARE_STATUS "Activated"
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

#define _DEV

using response_t = std::function<void(drogon::HttpResponsePtr const&)>;
using Multer = drogon::HttpFile;

using namespace std::chrono_literals;