#include "wishlist_service.hpp"
#include <exception/exception.hpp>

namespace gaboot
{
	wishlist_service::wishlist_service()
	{
		this->load_cache();
	}
	wishlist_service::~wishlist_service()
	{
		m_cache_wishlist.clear();
	}
	HttpResponsePtr wishlist_service::create(HttpRequestPtr const& req)
	{
		try
		{
			const auto& data = req->getJsonObject();

			if (!data) return BadRequestException().response();

			validator schema({
				{"productId", "type:number|required|minLength:3|numberOnly"},
				{"category", "type:string|required|minLength:3|aphabetOnly"}
			});

			Wishlists wishlist(*data);
			wishlist.setCreatedat(trantor::Date::now());
			wishlist.setUpdatedat(trantor::Date::now());

			if (!schema.validate(wishlist.toJson(), m_error))
			{
				return BadRequestException(m_error).response();
			}

			db().insert(wishlist);

			m_cache_wishlist.clear();

			m_response.m_message = "Create wishlist success";
			m_response.m_success = true;

			auto response = HttpResponse::newHttpJsonResponse(m_response.to_json());
			response->setStatusCode(k201Created);

			return response;
		}
		catch (const std::exception& e)
		{
			std::string error = fmt::format("Unable create wishlist data, error caught on {}", e.what());

			return CustomException<k500InternalServerError>(error).response();
		}
	}
    HttpResponsePtr wishlist_service::findAll(HttpRequestPtr const& req)
    {
		try
		{
			auto& limitParam = req->getParameter("limit");
			auto& pageParam = req->getParameter("page");

			const size_t limit = limitParam.empty() && !util::is_numeric(limitParam) ? 10 : stoull(limitParam);
			const size_t page = pageParam.empty() && !util::is_numeric(pageParam) ? 0 : stoull(pageParam) - 1;

			this->load_cache();

			const auto wishlists = m_cache_wishlist.limit(limit).offset(page * limit).find_all();

			if (wishlists.empty())
			{
				m_response.m_message = "Wishlist is empty";
				m_response.m_success = true;

				return HttpResponse::newHttpJsonResponse(m_response.to_json());
			}

			Json::Value data(Json::arrayValue);

			std::ranges::for_each(wishlists.begin(), wishlists.end(), [this](Wishlists const& wishlist) {
				m_response.m_data.append(wishlist.toJson());
			});

			const size_t lastPage = (wishlists.size() / (limit + (wishlists.size() % limit))) == 0 ? 0 : 1;

			m_response.m_message = "Success retreive wishlists data";
			m_response.m_success = true;
			m_response.m_data = data;
			m_response.m_last_page = lastPage;

			auto response = HttpResponse::newHttpJsonResponse(m_response.to_json());

			return response;
		}
		catch (const std::exception& e)
		{
			std::string error = fmt::format("Unable retrieve products data, error caught on {}", e.what());

			return CustomException<k500InternalServerError>(error).response();
		}
    }
	HttpResponsePtr wishlist_service::findOne(HttpRequestPtr const& req, std::string&& id)
	{
		try
		{
			if (id.empty() || !util::is_numeric(id))
			{
				return BadRequestException("Requirement doesn't match").response();
			}

			this->load_cache();

			const auto user = m_cache_wishlist.find(stoll(id));

			if (!user) return NotFoundException("Unable retrieve wishlist detail").response();

			m_response.m_message = "Success retrieve wishlist data";
			m_response.m_success = true;
			m_response.m_data = user->toJson();

			return HttpResponse::newHttpJsonResponse(m_response.to_json());
		}
		catch (const std::exception& e)
		{
			std::string error = fmt::format("Cannot retrieve wishlist data, error caught on {}", e.what());

			return CustomException<k500InternalServerError>(error).response();
		}
	}
	HttpResponsePtr wishlist_service::update(HttpRequestPtr const& req, std::string&& id)
	{
		try
		{
			const auto& json = req->getJsonObject();

			if (!json) return BadRequestException().response();

			if (id.empty() || !util::is_numeric(id))
			{
				return BadRequestException("Parameters requirement doesn't match").response();
			}

			const auto wishlist = m_cache_wishlist.find(stoll(id));

			wishlist->updateByJson(*json);
			wishlist->setId(stoll(id));
			wishlist->setUpdatedat(trantor::Date::now());

			this->load_cache();

			if (!m_cache_wishlist.update(stoll(id), *wishlist))
				return BadRequestException("Unable to update non-existing record").response();

			if (auto record = db().updateFuture(*wishlist).get(); !record)
				return BadRequestException("Unable to update non-existing record").response();

			m_response.m_data = wishlist->toJson();
			m_response.m_message = "Success update wishlist data.";
			m_response.m_success = true;

			auto response = HttpResponse::newHttpJsonResponse(m_response.to_json());
			return response;
		}
		catch (const std::exception& e)
		{
			return CustomException<k500InternalServerError>(fmt::format("Unable to update data, error caught on {}", e.what())).response();
		}
	}
	HttpResponsePtr wishlist_service::remove(HttpRequestPtr const& req, std::string&& id)
	{
		if (id.empty() || !util::is_numeric(id))
		{
			return BadRequestException("Parameters requirement doesn't match").response();
		}

		try
		{
			this->load_cache();

			if (m_cache_wishlist.remove(stoll(id)))
				return NotFoundException("Unable to delete non-existing record").response();

			if (auto record = db().deleteByPrimaryKey(stoll(id)); !record)
				return NotFoundException("Unable to delete non-existing record").response();

			m_response.m_message = "Delete wishlist successfully";
			m_response.m_success = true;

			return HttpResponse::newHttpJsonResponse(m_response.to_json());
		}
		catch (const DrogonDbException& e)
		{
			m_response.m_message = fmt::format("Failed delete wishlist, error caught on {}", e.base().what());
			m_response.m_success = false;

			auto response = HttpResponse::newHttpJsonResponse(m_response.to_json());
			response->setStatusCode(k500InternalServerError);

			return response;
		}
	}
}