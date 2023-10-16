#pragma once

#include <drogon/drogon.h>

#pragma warning(push)
#pragma warning(disable:4996)
#include <bcrypt.h>
#pragma warning(pop)

#define ADD_LISTENER(IP, PORT) drogon::app().addListener(IP, PORT)
#define ADD_CONFIG(JSON) drogon::app().loadConfigFile(JSON)
#define SERVER_RUN drogon::app().run()

#define DATABASE_CLIENT drogon::app().getDbClient()

using response_t = std::function<void(drogon::HttpResponsePtr const&)>;