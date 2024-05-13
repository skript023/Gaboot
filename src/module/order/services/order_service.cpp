#include "order_service.hpp"
#include "exception/exception.hpp"
#include "order/enums/order_status.hpp"

namespace gaboot
{
	HttpResponsePtr order_service::create(HttpRequestPtr const& req)
	{
		TRY_CLAUSE
		{
			auto& json = req->getJsonObject();
			
			Orders order;
			order.setCustomerId((*json)["customer_id"].asString());
			order.setName((*json)["name"].asString());
			order.setStatus(OrderStatus::OPEN);
			order.setGrandTotal((*json)["grand_total"].asDouble());
			order.setExpired(trantor::Date().after(24 * 3600).toDbStringLocal());
			order.setTotalItem((*json)["total_item"].asInt());
			order.setTotalPrice((*json)["total_price"].asInt());
			order.setDiscount(0.0);
			order.setCreatedAt(trantor::Date().now());
			order.setUpdatedAt(trantor::Date().now());

			db().insert(order);
			m_cache_order.clear();

			m_response.m_message = "Success create order";
			m_response.m_success = true;

			return HttpResponse::newHttpJsonResponse(m_response.to_json());
			
		} EXCEPT_CLAUSE
	}
	HttpResponsePtr order_service::findAll(HttpRequestPtr const& req)
	{
		TRY_CLAUSE
		{
			auto& limitParam = req->getParameter("limit");
			auto& pageParam = req->getParameter("page");
			auto& customer = req->getParameter("customerId");

			const size_t limit = limitParam.empty() && !util::is_numeric(limitParam) ? 10 : stoull(limitParam);
			const size_t page = pageParam.empty() && !util::is_numeric(pageParam) ? 0 : stoull(pageParam) - 1;

			auto callback = [customer](const Orders& entry) -> bool { return entry.getValueOfCustomerId() == customer; };
			auto orders = customer.empty() ? db().limit(limit).offset(page * limit).findAll() : db().findBy(Criteria(Orders::Cols::_customer_id, CompareOperator::EQ, customer));

			if (orders.empty())
			{
				m_response.m_message = "Orders data is empty 0 data found";
				m_response.m_success = true;

				return HttpResponse::newHttpJsonResponse(m_response.to_json());
			}

			m_response.m_data = orders;

			m_response.m_message = "Success retreive orders data";
			m_response.m_success = true;


			return HttpResponse::newHttpJsonResponse(m_response.to_json());
		} EXCEPT_CLAUSE
	}
	HttpResponsePtr order_service::findOne(HttpRequestPtr const&, std::string&& id)
	{
		TRY_CLAUSE
		{
			if (id.empty())
			{
				throw BadRequestException("Parameter is invalid");
			}

			auto order = db().findByPrimaryKey(id);

			m_response.m_message = "Success retrieve order data";
			m_response.m_success = true;
			m_response.m_data = order;

			return HttpResponse::newHttpJsonResponse(m_response.to_json());
		} EXCEPT_CLAUSE
	}
	HttpResponsePtr order_service::update(HttpRequestPtr const& req, std::string&& id)
	{
		TRY_CLAUSE
		{
			auto & json = req->getJsonObject();

			if (id.empty())
				throw BadRequestException("Parameter is invalid");

			this->load_cache();

			auto order = m_cache_order.find(id);

			order->updateByJson(*json);

			db().update(*order);

			m_cache_order.clear();

			m_response.m_message = "Success update order data.";
			m_response.m_success = true;

			return HttpResponse::newHttpJsonResponse(m_response.to_json());

		} EXCEPT_CLAUSE
	}
}