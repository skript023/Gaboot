#pragma once

#include <pch.h>

#include "util/validator.hpp"
#include "interfaces/response.hpp"
#include "module/category/models/Categories.h"

namespace gaboot
{
	class category_service
	{
	public:
		explicit category_service() = default;
		virtual ~category_service() = default;

		category_service(category_service const& that) = delete;
		category_service& operator=(category_service const& that) = delete;

		category_service(category_service&& that) = delete;
		category_service& operator=(category_service&& that) = delete;
	};
}