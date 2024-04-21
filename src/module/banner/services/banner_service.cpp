#include "banner_service.hpp"

#include <exception/exception.hpp>
#include <uploader/upload.hpp>

namespace gaboot
{
	banner_service::banner_service()
	{
		this->load_cache();
	}
	banner_service::~banner_service()
	{
		m_cache_banner.clear();
	}
	HttpResponsePtr banner_service::findAll(HttpRequestPtr const& req)
	{
		TRY_CLAUSE
		{
			auto & limitParam = req->getParameter("limit");
			auto& pageParam = req->getParameter("page");

			const size_t limit = limitParam.empty() && !util::is_numeric(limitParam) ? 10 : stoull(limitParam);
			const size_t page = pageParam.empty() && !util::is_numeric(pageParam) ? 0 : stoull(pageParam) - 1;

			this->load_cache();

			const auto banners = m_cache_banner.limit(limit).offset(page * limit).find_all();

			if (banners.empty())
			{
				m_response.m_message = "0 banner found";
				m_response.m_success = true;

				return HttpResponse::newHttpJsonResponse(m_response.to_json());
			}

			Json::Value data(Json::arrayValue);


			const size_t lastPage = (banners.size() / (limit + (banners.size() % limit))) == 0 ? 0 : 1;

			m_response.m_message = "Success retreive banners data";
			m_response.m_data = banners;
			m_response.m_success = true;
			m_response.m_last_page = lastPage;

			return HttpResponse::newHttpJsonResponse(m_response.to_json());
		} EXCEPT_CLAUSE
	}
	HttpResponsePtr banner_service::findOne(HttpRequestPtr const& req, std::string&& id)
	{
		TRY_CLAUSE
		{
			if (id.empty())
			{
				throw BadRequestException("Requirement doesn't match");
			}

			this->load_cache();

			auto banner = m_cache_banner.find(id);

			if (!banner) throw NotFoundException("Unable retrieve banner detail");

			m_response.m_message = "Success retrieve banner data";
			m_response.m_success = true;
			m_response.m_data = banner;

			return HttpResponse::newHttpJsonResponse(m_response.to_json());
		} EXCEPT_CLAUSE
	}
	HttpResponsePtr banner_service::getImage(HttpRequestPtr const& req, std::string&& id)
	{
		TRY_CLAUSE
		{
			Banners banner;

			if (id.empty())
			{
				LOG(WARNING) << "ID is empty or ID is not numeric";

				throw BadRequestException("Parameters requirement doesn't match");
			}

			this->load_cache();

			if (!m_cache_banner.find(id, &banner))
			{
				std::filesystem::path file(*banner.getImagePath());

				if (!std::filesystem::exists(file))
				{
					LOG(WARNING) << "File at " << file.lexically_normal() << " doesn't exist in server";

					m_response.m_message = "Unable to retreive banner picture, please upload your banner picture";
					m_response.m_success = false;

					auto response = HttpResponse::newHttpJsonResponse(m_response.to_json());
					response->setStatusCode(k404NotFound);

					return response;
				}

				if (auto image = banner.getImagePath(); image && !image->empty())
					return HttpResponse::newFileResponse(*banner.getImagePath());
			}

			throw NotFoundException("Unable to retreive banner image");
		} EXCEPT_CLAUSE
	}
	HttpResponsePtr banner_service::getThumbnail(HttpRequestPtr const& req, std::string&& id)
	{
		TRY_CLAUSE
		{
			Banners banner;

			if (id.empty())
			{
				LOG(WARNING) << "ID is empty or ID is not numeric";

				throw BadRequestException("Parameters requirement doesn't match");
			}

			this->load_cache();

			if (m_cache_banner.find(id, &banner))
			{
				std::filesystem::path file(*banner.getThumbnailPath());

				if (!std::filesystem::exists(file))
				{
					LOG(WARNING) << "File at " << file.lexically_normal() << " doesn't exist in server";

					m_response.m_message = "Unable to retreive banner picture, please upload your banner picture";
					m_response.m_success = false;

					auto response = HttpResponse::newHttpJsonResponse(m_response.to_json());
					response->setStatusCode(k404NotFound);

					return response;
				}

				if (auto image = banner.getImagePath(); image && !image->empty())
					return HttpResponse::newFileResponse(*banner.getThumbnailPath());
			}

			throw NotFoundException("Unable to retreive banner image");
		} EXCEPT_CLAUSE
	}
}