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
		TRY_CLAUSE
		{
			const auto& data = req->getJsonObject();

			if (!data) throw BadRequestException();

			validator schema({
				{"productId", "type:number|required|minLength:3|numberOnly"},
				{"category", "type:string|required|minLength:3|aphabetOnly"}
			});

			Wishlists wishlist(*data);
			wishlist.setCreatedat(trantor::Date::now());
			wishlist.setUpdatedat(trantor::Date::now());

			if (!schema.validate(wishlist.toJson(), m_error))
			{
				throw BadRequestException(m_error);
			}

			db().insert(wishlist);

			m_cache_wishlist.clear();

			m_response.m_message = "Create wishlist success";
			m_response.m_success = true;

			auto response = HttpResponse::newHttpJsonResponse(m_response.to_json());
			response->setStatusCode(k201Created);

			return response;
		} EXCEPT_CLAUSE
	}
    HttpResponsePtr wishlist_service::findAll(HttpRequestPtr const& req)
    {
		TRY_CLAUSE
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

			std::ranges::for_each(wishlists.begin(), wishlists.end(), [this](Wishlists const& wishlist) {
				m_response.m_data.append(wishlist.toJson());
			});

			const size_t lastPage = (wishlists.size() / (limit + (wishlists.size() % limit))) == 0 ? 0 : 1;

			m_response.m_message = "Success retreive wishlists data";
			m_response.m_success = true;
			m_response.m_last_page = lastPage;

			return HttpResponse::newHttpJsonResponse(m_response.to_json());
		} EXCEPT_CLAUSE
    }
	HttpResponsePtr wishlist_service::findOne(HttpRequestPtr const& req, std::string&& id)
	{
		TRY_CLAUSE
		{
			if (id.empty() || !util::is_numeric(id))
			{
				throw BadRequestException("Requirement doesn't match");
			}

			this->load_cache();

			const auto user = m_cache_wishlist.find(stoll(id));

			if (!user) throw NotFoundException("Unable retrieve wishlist detail");

			m_response.m_message = "Success retrieve wishlist data";
			m_response.m_success = true;
			m_response.m_data = user->toJson();

			return HttpResponse::newHttpJsonResponse(m_response.to_json());
		} EXCEPT_CLAUSE
	}
	HttpResponsePtr wishlist_service::update(HttpRequestPtr const& req, std::string&& id)
	{
		TRY_CLAUSE
		{
			const auto& json = req->getJsonObject();

			if (!json) throw BadRequestException();

			if (id.empty() || !util::is_numeric(id))
			{
				throw BadRequestException("Parameters requirement doesn't match");
			}

			const auto wishlist = m_cache_wishlist.find(stoll(id));

			wishlist->updateByJson(*json);
			wishlist->setId(stoll(id));
			wishlist->setUpdatedat(trantor::Date::now());

			this->load_cache();

			if (!m_cache_wishlist.update(stoll(id), *wishlist) || !db().updateFuture(*wishlist).get())
				throw BadRequestException("Unable to update non-existing record");

			m_response.m_data = wishlist->toJson();
			m_response.m_message = "Success update wishlist data.";
			m_response.m_success = true;

			return HttpResponse::newHttpJsonResponse(m_response.to_json());
		} EXCEPT_CLAUSE
	}
	HttpResponsePtr wishlist_service::remove(HttpRequestPtr const& req, std::string&& id)
	{

		TRY_CLAUSE
		{
			if (id.empty() || !util::is_numeric(id))
			{
				throw BadRequestException("Parameters requirement doesn't match");
			}

			this->load_cache();

			if (m_cache_wishlist.remove(stoll(id)))
				throw NotFoundException("Unable to delete non-existing record");

			if (auto record = db().deleteByPrimaryKey(stoll(id)); !record)
				throw NotFoundException("Unable to delete non-existing record");

			m_response.m_message = "Delete wishlist successfully";
			m_response.m_success = true;

			return HttpResponse::newHttpJsonResponse(m_response.to_json());
		} EXCEPT_CLAUSE
	}
}