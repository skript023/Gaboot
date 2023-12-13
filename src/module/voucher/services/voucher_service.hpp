#pragma once
#include <pch.h>

using namespace drogon;

namespace gaboot
{
	class voucher_service
	{
	public:
		explicit voucher_service() = default;
		~voucher_service() noexcept = default;

		voucher_service(voucher_service const&) = delete;
		voucher_service& operator=(voucher_service const&) = delete;

		voucher_service(voucher_service&&) = delete;
		voucher_service& operator=(voucher_service&&) = delete;

		HttpResponsePtr create(HttpRequestPtr const& req);
		HttpResponsePtr findAll(HttpRequestPtr const& req);
		HttpResponsePtr findOne(HttpRequestPtr const& req, std::string&& id);
		HttpResponsePtr update(HttpRequestPtr const& req, std::string&& id);
		HttpResponsePtr remove(HttpRequestPtr const& req, std::string&& id);
	};
}