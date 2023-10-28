#pragma once
#include <pch.h>

namespace gaboot
{
	class cart_service
	{
	public:
		explicit cart_service() = default;
		virtual ~cart_service() noexcept = default;

		cart_service(cart_service const& that) = delete;
		cart_service& operator=(cart_service const& that) = delete;

		cart_service(cart_service&& that) = delete;
		cart_service& operator=(cart_service&& that) = delete;
	};
}