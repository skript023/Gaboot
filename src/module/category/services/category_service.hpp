#pragma once

#include <pch.h>

#include "util/validator.hpp"
#include "interfaces/response.hpp"
#include "module/category/models/Categories.h"

using namespace drogon;
using namespace orm;
using namespace drogon_model::gaboot;

namespace gaboot
{
	class category_service
	{
		Mapper<Categories> db() { return Mapper<Categories>(DATABASE_CLIENT); }
		std::map<Json::Value::Members, std::string> columnMapping = {
			{{Categories::Cols::_name}, "name"},
			{{Categories::Cols::_description}, "description"},
			{{Categories::Cols::_imagePath}, "imagePath"},
			{{Categories::Cols::_thumbnailPath}, "thumbnailPath"},
			{{Categories::Cols::_updatedAt}, "updatedAt"},
			{{Categories::Cols::_createdAt}, "createdAt"}
		};
	public:
		explicit category_service() = default;
		virtual ~category_service() = default;

		category_service(category_service const& that) = delete;
		category_service& operator=(category_service const& that) = delete;

		category_service(category_service&& that) = delete;
		category_service& operator=(category_service&& that) = delete;

		HttpResponsePtr create(HttpRequestPtr const&);
		HttpResponsePtr findAll(HttpRequestPtr const&);
		HttpResponsePtr findOne(HttpRequestPtr const&, std::string&& id);
		HttpResponsePtr update(HttpRequestPtr const&, std::string&& id);
		HttpResponsePtr remove(HttpRequestPtr const&, std::string&& id);
	private:
		response_data m_response;
		std::string m_error;
		Json::Value m_data;
	};
}