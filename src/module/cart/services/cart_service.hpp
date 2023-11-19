#pragma once
#include <pch.h>

#include "util/validator.hpp"
#include "interfaces/response.hpp"
#include "cart/models/Carts.h"

using namespace drogon;
using namespace orm;
using namespace drogon_model::gaboot;

namespace gaboot
{
	class cart_service
	{
		Mapper<Carts> db() { return Mapper<Carts>(DATABASE_CLIENT); }
	public:
		explicit cart_service() = default;
		virtual ~cart_service() noexcept = default;

		cart_service(cart_service const& that) = delete;
		cart_service& operator=(cart_service const& that) = delete;

		cart_service(cart_service&& that) = delete;
		cart_service& operator=(cart_service&& that) = delete;
	};
}