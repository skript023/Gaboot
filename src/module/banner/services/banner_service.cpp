#include "banner_service.hpp"

#include <exception/exception.hpp>
#include <uploader/upload.hpp>

namespace gaboot
{
	banner_service::banner_service():
		m_database(std::make_unique<Mapper<Banners>>(DATABASE_CLIENT)),
		m_response(std::make_unique<response_data<BannerResponse>>())
	{
		LOG(INFO) << "Category service registered.";
	}
	banner_service::~banner_service()
	{
		m_database.reset();
		m_response.reset();
	}
	HttpResponsePtr banner_service::findAll(HttpRequestPtr const& req)
	{
		auto& limitParam = req->getParameter("limit");
		auto& pageParam = req->getParameter("page");

		const size_t limit = limitParam.empty() && !util::is_numeric(limitParam) ? 10 : stoull(limitParam);
		const size_t page = pageParam.empty() && !util::is_numeric(pageParam) ? 0 : stoull(pageParam) - 1;

		const auto banners = m_database->orderBy(Banners::Cols::_name).limit(limit).offset(page * limit).findAll();

		if (banners.empty())
		{
			m_response->m_message = "0 banner found";
			m_response->m_success = true;

			return HttpResponse::newHttpJsonResponse(m_response->to_json());
		}

		Json::Value data(Json::arrayValue);


		const size_t lastPage = (banners.size() / (limit + (banners.size() % limit))) == 0 ? 0 : 1;

		m_response->m_message = "Success retrieve banners data";
		m_response->m_data = banners;
		m_response->m_success = true;
		m_response->m_last_page = lastPage;

		return HttpResponse::newHttpJsonResponse(m_response->to_json());
	}
	HttpResponsePtr banner_service::findOne(HttpRequestPtr const& req, std::string&& id)
	{
		if (id.empty())
		{
			throw BadRequestException("Requirement doesn't match");
		}

		auto banner = m_database->findByPrimaryKey(id);

		m_response->m_message = "Success retrieve banner data";
		m_response->m_success = true;
		m_response->m_data = banner;

		return HttpResponse::newHttpJsonResponse(m_response->to_json());
	}
	HttpResponsePtr banner_service::getImage(HttpRequestPtr const& req, std::string&& id)
	{
		if (id.empty())
		{
			LOG(WARNING) << "ID is empty or ID is not numeric";

			throw BadRequestException("Parameters requirement doesn't match");
		}

		Banners banner = m_database->findByPrimaryKey(id);

		std::filesystem::path file(*banner.getImagePath());

		if (!std::filesystem::exists(file))
		{
			LOG(WARNING) << "File at " << file.lexically_normal() << " doesn't exist in server";

			m_response->m_message = "Unable to retreive banner picture, please upload your banner picture";
			m_response->m_success = false;

			auto response = HttpResponse::newHttpJsonResponse(m_response->to_json());
			response->setStatusCode(k404NotFound);

			return response;
		}

		return HttpResponse::newFileResponse(*banner.getImagePath());
	}
	HttpResponsePtr banner_service::getThumbnail(HttpRequestPtr const& req, std::string&& id)
	{
		if (id.empty())
		{
			LOG(WARNING) << "ID is empty or ID is not numeric";

			throw BadRequestException("Parameters requirement doesn't match");
		}

		Banners banner = m_database->findByPrimaryKey(id);

		std::filesystem::path file(*banner.getThumbnailPath());

		if (!std::filesystem::exists(file))
		{
			LOG(WARNING) << "File at " << file.lexically_normal() << " doesn't exist in server";

			m_response->m_message = "Unable to retreive banner picture, please upload your banner picture";
			m_response->m_success = false;

			auto response = HttpResponse::newHttpJsonResponse(m_response->to_json());
			response->setStatusCode(k404NotFound);

			return response;
		}

		return HttpResponse::newFileResponse(*banner.getThumbnailPath());
	}
}