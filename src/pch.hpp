#pragma once

#define JWT_DISABLE_PICOJSON

#include <iostream>
#include <windows.h>
#include <TlHelp32.h>

#include <string>
#include <thread>
#include <stack>
#include <regex>
#include <array>
#include <chrono>
#include <xmemory>
#include <fstream>
#include <filesystem>

#include <drogon/drogon.h>
#include <nlohmann/json.hpp>
#include <jwt-cpp/jwt.h>
#include <cpr/cpr.h>

#include "util/gaboot.hpp"

#include <jwt-cpp/traits/nlohmann-json/traits.h>
#pragma warning(push)
#pragma warning(disable:4996)
#include <bcrypt.h>
#pragma warning(pop)
#define ADD_LISTENER(IP, PORT) drogon::app().addListener(IP, PORT)
#define ADD_CONFIG(JSON) drogon::app().loadConfigFile(JSON)
#define SERVER_RUN drogon::app().run()

#define DATABASE_CLIENT drogon::app().getDbClient()
#define SECRET "asasasasaaaaxxx"

using response_t = std::function<void(drogon::HttpResponsePtr const&)>;

using namespace std::chrono_literals;