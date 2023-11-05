#include "product_service.hpp"
#include "util/exception.hpp"
#include <util/upload.hpp>

namespace gaboot
{
	HttpResponsePtr product_service::findAll(HttpRequestPtr const& req)
	{
		try
		{
			auto& limitParam = req->getParameter("limit");
			auto& pageParam = req->getParameter("page");

			const size_t limit = limitParam.empty() && !util::is_numeric(limitParam) ? 10 : stoull(limitParam);
			const size_t page = pageParam.empty() && !util::is_numeric(pageParam) ? 0 : stoull(pageParam) - 1;

			const auto products = db().orderBy(MasterProducts::Cols::_name).limit(limit).offset(page * limit).findFutureAll().get();

			if (products.empty())
			{
				return NotFoundException("No data retrieved").response();
			}

			Json::Value res(Json::arrayValue);

			for (const auto& user : products)
			{
				res.append(user.toJson());
			}

			const size_t lastPage = (products.size() / (limit + (products.size() % limit))) == 0 ? 0 : 1;

			m_response.m_message = "Success retreive products data";
			m_response.m_success = true;
			m_response.m_data = res;
			m_response.m_last_page = lastPage;

			return HttpResponse::newHttpJsonResponse(m_response.to_json());
		}
		catch (const DrogonDbException& e)
		{
			m_response.m_message = fmt::format("Unable retrieve products data, error caught on {}", e.base().what());
			m_response.m_success = false;

			auto response = HttpResponse::newHttpJsonResponse(m_response.to_json());
			response->setStatusCode(k500InternalServerError);

			return response;
		}
	}
	HttpResponsePtr product_service::create(HttpRequestPtr const& req)
	{
		try
		{
			MultiPartParser fileUpload;

			if (fileUpload.parse(req) != 0 || fileUpload.getFiles().size() == 0)
			{
				return BadRequestException("Requirement doesn't match").response();
			}

			auto& file = fileUpload.getFiles()[0];

			if (!util::multipart_tojson(fileUpload, m_data)) return BadRequestException("Unknown error").response();

			validator schema({
				{"name", "type:string|required|minLength:3|alphanum"},
				{"description", "type:string|required|minLength:3|alphabetOnly"},
				{"price", "type:number|required|numberOnly"},
				{"stock", "type:number|required|numberOnly"}
			});

			m_data["price"] = stod(m_data["price"].asString());
			m_data["stock"] = stoi(m_data["stock"].asString());

			MasterProducts product(m_data);
			ProductImages productImage;
			upload_file upload(file, product.getValueOfName(), "products");
			productImage.setImagepath(upload.get_image_path());
			productImage.setThumbnailpath(upload.get_thumbnail_path());
			
			productImage.setCreatedAt(trantor::Date::now());
			productImage.setUpdatedAt(trantor::Date::now());

			product.setCreatedat(trantor::Date::now());
			product.setUpdatedat(trantor::Date::now());

			if (!schema.validate(product.toJson(), m_error))
			{
				return BadRequestException(m_error).response();
			}

			db().insert(product);
			productImage.setProductid(product.getPrimaryKey());
			db_images().insert(productImage);

			upload.save();

			m_response.m_message = "Create product success";
			m_response.m_success = true;

			auto response = HttpResponse::newHttpJsonResponse(m_response.to_json());

			return response;
		}
		catch (const DrogonDbException& e)
		{
			LOG(WARNING) << e.base().what();

			m_response.m_message = e.base().what();
			m_response.m_success = false;

			auto response = HttpResponse::newHttpJsonResponse(m_response.to_json());
			response->setStatusCode(HttpStatusCode::k500InternalServerError);

			return response;
		}
	}
	HttpResponsePtr product_service::findOne(HttpRequestPtr const& req, std::string&& id)
	{
		try
		{
			if (id.empty() || !util::is_numeric(id))
			{
				return BadRequestException("Parameters requirement doesn't match").response();
			}

			const auto product = db().findFutureByPrimaryKey(stoll(id)).get();

			if (!product.getId()) return NotFoundException("Product data is empty 0 data found").response();

			m_response.m_message = "Success retrieve products data";
			m_response.m_success = true;
			m_response.m_data = product.toJson();

			auto response = HttpResponse::newHttpJsonResponse(m_response.to_json());

			return response;
		}
		catch (const DrogonDbException& e)
		{
			m_response.m_message = fmt::format("Unable retrieve products data, error caught on {}", e.base().what());
			m_response.m_success = false;

			auto response = HttpResponse::newHttpJsonResponse(m_response.to_json());
			response->setStatusCode(k500InternalServerError);

			return response;
		}
	}
	HttpResponsePtr product_service::update(HttpRequestPtr const& req, std::string&& id)
	{
		try
		{
			MultiPartParser multipart;

			if (id.empty() || !util::is_numeric(id))
			{
				return BadRequestException("Parameters requirement doesn't match").response();
			}

			if (multipart.parse(req) != 0)
			{
				return BadRequestException("Requirement doesn't match").response();
			}

			auto& file = multipart.getFiles()[0];

			util::multipart_tojson(multipart, m_data);

			upload_file upload(file, std::to_string(trantor::Date::now().microSecondsSinceEpoch()), "products");

			m_data["updatedAt"] = trantor::Date::now().toDbStringLocal();

			if (multipart.getFiles().size() > 0 && util::allowed_image(file.getFileExtension().data()))
			{
				m_data_image["imagePath"] = upload.get_image_path();
				m_data_image["thumbnailPath"] = upload.get_thumbnail_path();
				m_data_image["updatedAt"] = trantor::Date::now().toDbStringLocal();
			}

			MasterProducts product(m_data);
			product.setId(stoll(id));
			product.setUpdatedat(trantor::Date::now());

			if (const auto record = db().update(product); !record)
			{
				return NotFoundException("Unable to update non-existing product").response();
			}

			if (multipart.getFiles().size() > 0 && util::allowed_image(file.getFileExtension().data()))
			{
				if (const auto record2 = db_images().updateBy(m_data_image.getMemberNames(),
					Criteria(ProductImages::Cols::_productId, CompareOperator::EQ, id),
					upload.get_image_path(),
					upload.get_thumbnail_path(),
					trantor::Date::now()
				); !record2)
				{
					return NotFoundException("Unable to update non-existing product").response();
				}
				upload.save();
			}

			m_response.m_data = product.toJson();
			m_response.m_message = "Success update customer data.";
			m_response.m_success = true;

			auto response = HttpResponse::newHttpJsonResponse(m_response.to_json());
			return response;
		}
		catch (const DrogonDbException& e)
		{
			m_response.m_message = fmt::format("Unable to update data, error caught on {}", e.base().what());
			m_response.m_success = false;

			auto response = HttpResponse::newHttpJsonResponse(m_response.to_json());
			response->setStatusCode(k500InternalServerError);

			LOG(WARNING) << e.base().what();

			return response;
		}
	}
	HttpResponsePtr product_service::remove(HttpRequestPtr const& req, std::string&& id)
	{
		try
		{
			if (id.empty() || !util::is_numeric(id))
			{
				return BadRequestException("Parameters requirement doesn't match").response();
			}

			const auto record = db().deleteFutureByPrimaryKey(stoll(id)).get();

			if (record != 0)
			{
				m_response.m_message = fmt::format("Delete user on {} successfully", record);
				m_response.m_success = true;

				auto response = HttpResponse::newHttpJsonResponse(m_response.to_json());
				return response;
			}
			
			return NotFoundException("Unable to delete non-existing data").response();
		}
		catch (const DrogonDbException& e)
		{
			m_response.m_message = fmt::format("Failed delete user, error caught on {}", e.base().what());
			m_response.m_success = false;

			auto response = HttpResponse::newHttpJsonResponse(m_response.to_json());
			response->setStatusCode(k500InternalServerError);

			return response;
		}
	}
}