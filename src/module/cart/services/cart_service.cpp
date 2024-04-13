#include "cart_service.hpp"

#include <util/validator/validator.hpp>
#include <util/exception/exception.hpp>

namespace gaboot
{
    cart_service::cart_service()
    {
        this->load_cache();
    }

    cart_service::~cart_service() noexcept
    {
        m_cache_cart.clear();
    }

    HttpResponsePtr cart_service::create(HttpRequestPtr const &req)
    {
        TRY_CLAUSE
        {
            auto& json = req->getJsonObject();

            Carts cart;

            cart.setCustomerId((*json)["customer_id"].asString());
            cart.setProductId((*json)["product_id"].asString());
            cart.setPrice((*json)["price"].asDouble());
            cart.setQuantity((*json)["quantity"].asInt());
            cart.setCreatedAt(trantor::Date::now());
            cart.setUpdatedAt(trantor::Date::now());

            validator schema({
                {"customer_id", "type:string|required"},
                {"product_id", "type:string|required"},
                {"price", "type:number|required|numberOnly"},
                {"quantity", "type:number|required|numberOnly"},
            });

            if (!schema.validate(*json, m_response.m_message))
                throw BadRequestException(m_response.m_message);

            db().insert(cart);

            m_response.m_message = "Success added to cart";
            m_response.m_success = true;
            
            return HttpResponse::newHttpJsonResponse(m_response.to_json());
        } EXCEPT_CLAUSE
    }
    HttpResponsePtr cart_service::findAll(HttpRequestPtr const &req)
    {
        TRY_CLAUSE
        {
            auto& limitParam = req->getParameter("limit");
            auto& pageParam = req->getParameter("page");
            auto& customer = req->getParameter("customerId");
            auto& product = req->getParameter("productId");

            const size_t limit = limitParam.empty() && !util::is_numeric(limitParam) ? 10 : stoull(limitParam);
            const size_t page = pageParam.empty() && !util::is_numeric(pageParam) ? 0 : stoull(pageParam) - 1;

            auto customerCallback = [customer](const Carts& entry) -> bool { return entry.getValueOfCustomerId() == customer; };
            auto productCallback = [product](const Carts& entry) -> bool { return entry.getValueOfProductId() == product; };

            this->load_cache();

            auto carts = !customer.empty() ?  m_cache_cart.find(customerCallback) : !product.empty() ? m_cache_cart.find(productCallback) : m_cache_cart.limit(limit).offset(page * limit).find_all();

            if (carts.empty())
            {
                m_response.m_message = "Carts data is empty 0 data found";
                m_response.m_success = true;

                return HttpResponse::newHttpJsonResponse(m_response.to_json());
            }

            std::ranges::for_each(carts.begin(), carts.end(), [this](Carts const& cart) {
                CartsResponse c = cart;
                m_response.m_data.append(c.to_json());
			});

            m_response.m_message = "Success retreive carts data";
			m_response.m_success = true;

			return HttpResponse::newHttpJsonResponse(m_response.to_json());
        } EXCEPT_CLAUSE
    }
    HttpResponsePtr cart_service::findOne(HttpRequestPtr const &req, std::string &&id)
    {
        TRY_CLAUSE
        {
            if (id.empty())
			{
				throw BadRequestException("Parameter is invalid");
			}

			this->load_cache();

            CartsResponse cart = m_cache_cart.find(id);

			if (!&cart) throw NotFoundException("Cart data is not found");

			m_response.m_message = "Success retrieve cart data";
			m_response.m_success = true;
            m_response.m_data = cart.to_json();

			return HttpResponse::newHttpJsonResponse(m_response.to_json());
        } EXCEPT_CLAUSE
    }
    HttpResponsePtr cart_service::update(HttpRequestPtr const &req, std::string &&id)
    {
        TRY_CLAUSE
        {
            auto json = req->getJsonObject();

            if (id.empty())
                throw BadRequestException("Parameter is invalid");

            this->load_cache();

            auto cart = m_cache_cart.find(id);

            cart->updateByJson(*json);

            if (!db().update(*cart) || !m_cache_cart.update(id, *cart))
                throw BadRequestException("Unable to update non-existing cart");

            m_cache_cart.clear();

            m_response.m_message = "Success update cart data.";
			m_response.m_success = true;

			return HttpResponse::newHttpJsonResponse(m_response.to_json());
        } EXCEPT_CLAUSE
    }
    HttpResponsePtr cart_service::remove(HttpRequestPtr const &req, std::string &&id)
    {
        TRY_CLAUSE
        {
            if (id.empty())
                throw BadRequestException("Parameter is invalid");

            this->load_cache();

            if (!db().deleteByPrimaryKey(id) || !m_cache_cart.remove(id))
                throw BadRequestException("Unable delete non-existing cart");

            m_response.m_message = "Success delete cart";
			m_response.m_success = true;

			return HttpResponse::newHttpJsonResponse(m_response.to_json());
        } EXCEPT_CLAUSE
    }
}