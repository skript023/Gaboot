#pragma once
#include <pch.h>

#include "validator/validator.hpp"
#include "interfaces/response.hpp"

#include "banner/models/Banners.h"
#include "banner/dto/banner.dto.hpp"
#include "cache_manager/cache_handler.hpp"

using namespace drogon;
using namespace orm;
using namespace drogon_model::gaboot;

namespace gaboot
{
	class banner_service
	{
	public:
		explicit banner_service();
		~banner_service() noexcept;

		banner_service(banner_service const&) = delete;
		banner_service& operator=(banner_service const&) = delete;

		banner_service(banner_service&&) = delete;
		banner_service& operator=(banner_service&&) = delete;

		HttpResponsePtr findAll(HttpRequestPtr const& req);
		HttpResponsePtr findOne(HttpRequestPtr const& req, std::string&& id);
		HttpResponsePtr getImage(HttpRequestPtr const&, std::string&&);
		HttpResponsePtr getThumbnail(HttpRequestPtr const&, std::string&&);
	private:
		void load_cache()
		{
			if (m_cache_banner.empty() || m_cache_banner.expired())
			{
				auto banners = m_database->orderBy(Banners::Cols::_name).findFutureAll().get();
				m_cache_banner.cache_duration(5min);

				std::ranges::for_each(banners.begin(), banners.end(), [this](Banners category) {
					m_cache_banner.insert(*category.getId(), &category);
				});
			}
		}
	private:
		std::unique_ptr<response_data<BannerResponse>> m_response;
		cache_handler<Banners> m_cache_banner;
		std::unique_ptr<Mapper<Banners>> m_database;
	};
}