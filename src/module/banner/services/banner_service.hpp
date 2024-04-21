#pragma once
#include <pch.h>

using namespace drogon;

namespace gaboot
{
	class banner_service
	{
	public:
		explicit banner_service() = default;
		~banner_service() noexcept = default;

		banner_service(banner_service const&) = delete;
		banner_service& operator=(banner_service const&) = delete;

		banner_service(banner_service&&) = delete;
		banner_service& operator=(banner_service&&) = delete;

		HttpResponsePtr create(HttpRequestPtr const& req);
		HttpResponsePtr findAll(HttpRequestPtr const& req);
		HttpResponsePtr findOne(HttpRequestPtr const& req, std::string&& id);
		HttpResponsePtr update(HttpRequestPtr const& req, std::string&& id);
		HttpResponsePtr remove(HttpRequestPtr const& req, std::string&& id);
	};
}