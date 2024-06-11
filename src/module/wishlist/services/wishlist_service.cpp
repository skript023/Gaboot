#include "wishlist_service.hpp"
#include <exception/exception.hpp>

namespace gaboot
{
	wishlist_service::wishlist_service():
		m_database(std::make_unique<Mapper<Wishlists>>(DATABASE_CLIENT)),
		m_response(std::make_unique<response_data<WishlistResponse>>())
	{
		LOG(INFO) << "Wishlist service registered.";
	}
	wishlist_service::~wishlist_service() noexcept
	{
		m_database.reset();
		m_response.reset();
	}
	HttpResponsePtr wishlist_service::create(HttpRequestPtr const& req)
	{
		const auto& data = req->getJsonObject();

		if (!data) throw BadRequestException();

		validator schema({
			{"productId", "type:number|required|minLength:3|numberOnly"},
			{"category", "type:string|required|minLength:3|aphabetOnly"}
			});

		Wishlists wishlist(*data);
		wishlist.setCreatedAt(trantor::Date::now());
		wishlist.setUpdatedAt(trantor::Date::now());

		if (!schema.validate(wishlist.toJson(), m_error))
		{
			throw BadRequestException(m_error);
		}

		m_database->insert(wishlist);

		m_response->m_message = "Create wishlist success";
		m_response->m_success = true;

		auto response = HttpResponse::newHttpJsonResponse(m_response->to_json());
		response->setStatusCode(k201Created);

		return response;
	}
    HttpResponsePtr wishlist_service::findAll(HttpRequestPtr const& req)
    {
		auto& limitParam = req->getParameter("limit");
		auto& pageParam = req->getParameter("page");

		const size_t limit = limitParam.empty() && !util::is_numeric(limitParam) ? 10 : stoull(limitParam);
		const size_t page = pageParam.empty() && !util::is_numeric(pageParam) ? 0 : stoull(pageParam) - 1;

		const auto wishlists = m_database->limit(limit).offset(page * limit).findAll();

		if (wishlists.empty())
		{
			m_response->m_message = "Wishlist is empty";
			m_response->m_success = true;

			return HttpResponse::newHttpJsonResponse(m_response->to_json());
		}

		const size_t lastPage = (wishlists.size() / (limit + (wishlists.size() % limit))) == 0 ? 0 : 1;

		m_response->m_data = wishlists;
		m_response->m_message = "Success retreive wishlists data";
		m_response->m_success = true;
		m_response->m_last_page = lastPage;

		return HttpResponse::newHttpJsonResponse(m_response->to_json());
    }
	HttpResponsePtr wishlist_service::findOne(HttpRequestPtr const& req, std::string&& id)
	{
		if (id.empty())
		{
			throw BadRequestException("Requirement doesn't match");
		}

		auto wishlist = m_database->findByPrimaryKey(id);

		m_response->m_message = "Success retrieve wishlist data";
		m_response->m_success = true;
		m_response->m_data = wishlist;

		return HttpResponse::newHttpJsonResponse(m_response->to_json());
	}
	HttpResponsePtr wishlist_service::update(HttpRequestPtr const& req, std::string&& id)
	{
		const auto& json = req->getJsonObject();

		if (!json) throw BadRequestException();

		if (id.empty())
		{
			throw BadRequestException("Parameters requirement doesn't match");
		}

		auto wishlist = m_database->findByPrimaryKey(id);

		wishlist.updateByJson(*json);
		wishlist.setId(id);
		wishlist.setUpdatedAt(trantor::Date::now());

		if (!m_database->updateFuture(wishlist).get())
			throw BadRequestException("Unable to update non-existing record");

		m_response->m_data = wishlist;
		m_response->m_message = "Success update wishlist data.";
		m_response->m_success = true;

		return HttpResponse::newHttpJsonResponse(m_response->to_json());
	}
	HttpResponsePtr wishlist_service::remove(HttpRequestPtr const& req, std::string&& id)
	{

		if (id.empty())
		{
			throw BadRequestException("Parameters requirement doesn't match");
		}

		if (auto record = m_database->deleteByPrimaryKey(id); !record)
			throw NotFoundException("Unable to delete non-existing record");

		m_response->m_message = "Delete wishlist successfully";
		m_response->m_success = true;

		return HttpResponse::newHttpJsonResponse(m_response->to_json());
	}
}