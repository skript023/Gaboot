#include "cart_service.hpp"

#include <util/validator/validator.hpp>
#include <util/exception/exception.hpp>

namespace gaboot
{
    cart_service::cart_service() :
        m_database(std::make_unique<Mapper<Carts>>(DATABASE_CLIENT)), m_response(std::make_unique<response_data<CartsResponse>>())
    {
        LOG(INFO) << "Cart service registered.";
    }
    cart_service::~cart_service() noexcept
    {
        m_database.reset();
        m_response.reset();
    }
    HttpResponsePtr cart_service::create(HttpRequestPtr const &req)
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

        if (!schema.validate(*json, m_response->m_message))
            throw BadRequestException(m_response->m_message);

        m_database->insert(cart);

        m_response->m_message = "Success added to cart";
        m_response->m_success = true;

        return HttpResponse::newHttpJsonResponse(m_response->to_json());
    }
    HttpResponsePtr cart_service::findAll(HttpRequestPtr const &req)
    {
        auto& limitParam = req->getParameter("limit");
        auto& pageParam = req->getParameter("page");
        auto& customer = req->getParameter("customerId");
        auto& product = req->getParameter("productId");

        const size_t limit = limitParam.empty() && !util::is_numeric(limitParam) ? 10 : stoull(limitParam);
        const size_t page = pageParam.empty() && !util::is_numeric(pageParam) ? 0 : stoull(pageParam) - 1;

        auto customerCallback = [customer](const Carts& entry) -> bool { return entry.getValueOfCustomerId() == customer; };
        auto productCallback = [product](const Carts& entry) -> bool { return entry.getValueOfProductId() == product; };

        auto carts = !customer.empty() ? m_database->findBy(Criteria(Carts::Cols::_product_id, CompareOperator::EQ, product)) : !product.empty() ? m_database->findBy(Criteria(Carts::Cols::_customer_id, CompareOperator::EQ, customer)) : m_database->limit(limit).offset(page * limit).orderBy(Carts::Cols::_id).findAll();

        if (carts.empty())
        {
            m_response->m_message = "Carts data is empty 0 data found";
            m_response->m_success = true;

            return HttpResponse::newHttpJsonResponse(m_response->to_json());
        }


        m_response->m_message = "Success retrieve carts data";
        m_response->m_data = carts;
        m_response->m_success = true;

        return HttpResponse::newHttpJsonResponse(m_response->to_json());
    }
    HttpResponsePtr cart_service::findOne(HttpRequestPtr const &req, std::string &&id)
    {
        if (id.empty())
        {
            throw BadRequestException("Parameter is invalid");
        }

        auto cart = m_database->findByPrimaryKey(id);

        m_response->m_message = "Success retrieve cart data";
        m_response->m_success = true;
        m_response->m_data = cart;

        return HttpResponse::newHttpJsonResponse(m_response->to_json());
    }
    HttpResponsePtr cart_service::update(HttpRequestPtr const &req, std::string &&id)
    {
        auto json = req->getJsonObject();

        if (id.empty())
            throw BadRequestException("Parameter is invalid");

        auto cart = m_database->findByPrimaryKey(id);

        cart.updateByJson(*json);

        if (!m_database->update(cart))
            throw BadRequestException("Unable to update non-existing cart");

        m_response->m_message = "Success update cart data.";
        m_response->m_success = true;

        return HttpResponse::newHttpJsonResponse(m_response->to_json());
    }
    HttpResponsePtr cart_service::remove(HttpRequestPtr const &req, std::string &&id)
    {
        if (id.empty())
            throw BadRequestException("Parameter is invalid");

        if (!m_database->deleteByPrimaryKey(id))
            throw BadRequestException("Unable delete non-existing cart");

        m_response->m_message = "Success delete cart";
        m_response->m_success = true;

        return HttpResponse::newHttpJsonResponse(m_response->to_json());
    }
}