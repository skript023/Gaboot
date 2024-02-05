#include "product_service.hpp"
#include "uploader/upload.hpp"
#include "exception/exception.hpp"

namespace gaboot
{
	product_service::product_service()
	{
		this->load_cache();
	}
	product_service::~product_service()
	{
		m_cache_product.clear();
	}
	HttpResponsePtr product_service::findAll(HttpRequestPtr const& req)
	{
		TRY_CLAUSE
		{
			auto& limitParam = req->getParameter("limit");
			auto& pageParam = req->getParameter("page");
			auto& categoryId = req->getParameter("categoryId");

			const size_t limit = limitParam.empty() && !util::is_numeric(limitParam) ? 10 : stoull(limitParam);
			const size_t page = pageParam.empty() && !util::is_numeric(pageParam) ? 0 : stoull(pageParam) - 1;

			this->load_cache();

			const auto products = categoryId.empty() ? m_cache_product.limit(limit).offset(page * limit).find_all() : m_cache_product.find([categoryId](const MasterProducts& entry) -> bool { return entry.getValueOfCategoryid() == stoi(categoryId); });

			if (products.empty())
			{
				m_response.m_message = "Product data is empty 0 data found";
				m_response.m_success = true;

				return HttpResponse::newHttpJsonResponse(m_response.to_json());
			}

			std::ranges::for_each(products.begin(), products.end(), [this](MasterProducts const& product) {
				m_response.m_data.append(product.toJson());
			});

			const size_t lastPage = (products.size() / (limit + (products.size() % limit))) == 0 ? 0 : 1;

			m_response.m_message = "Success retreive products data";
			m_response.m_success = true;
			m_response.m_last_page = lastPage;

			return HttpResponse::newHttpJsonResponse(m_response.to_json());
		} EXCEPT_CLAUSE
	}
	HttpResponsePtr product_service::create(HttpRequestPtr const& req)
	{
		TRY_CLAUSE
		{
			MultiPartParser fileUpload;

			if (fileUpload.parse(req) != 0 || fileUpload.getFiles().size() == 0)
			{
				throw BadRequestException("Requirement doesn't match");
			}

			auto& file = fileUpload.getFiles()[0];

			if (!util::multipart_tojson(fileUpload, m_data)) throw BadRequestException("Unknown error");

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
			upload_file upload(&file, product.getValueOfName(), "products");
			productImage.setImagepath(upload.get_image_path());
			productImage.setThumbnailpath(upload.get_thumbnail_path());

			productImage.setCreatedat(trantor::Date::now());
			productImage.setUpdatedat(trantor::Date::now());

			product.setPrice(m_data["price"].asDouble());
			product.setCreatedat(trantor::Date::now());
			product.setUpdatedat(trantor::Date::now());

			if (!schema.validate(product.toJson(), m_error))
			{
				throw BadRequestException(m_error);
			}

			db().insert(product);
			productImage.setProductid(product.getPrimaryKey());
			db_images().insert(productImage);

			m_cache_product.clear();

			upload.save();

			m_response.m_message = "Create product success";
			m_response.m_success = true;

			auto response = HttpResponse::newHttpJsonResponse(m_response.to_json());
			response->setStatusCode(k201Created);

			return response;
		} EXCEPT_CLAUSE
	}
	HttpResponsePtr product_service::findOne(HttpRequestPtr const& req, std::string&& id)
	{
		TRY_CLAUSE
		{
			if (id.empty() || !util::is_numeric(id))
			{
				throw BadRequestException("Parameters requirement doesn't match");
			}

			this->load_cache();

			const auto product = m_cache_product.find(stoull(id));

			if (!product) throw NotFoundException("Product data is empty 0 data found");

			m_response.m_message = "Success retrieve products data";
			m_response.m_success = true;
			m_response.m_data = product->toJson();

			return HttpResponse::newHttpJsonResponse(m_response.to_json());
		} EXCEPT_CLAUSE
	}
	HttpResponsePtr product_service::update(HttpRequestPtr const& req, std::string&& id)
	{
		TRY_CLAUSE
		{
			MultiPartParser multipart;

			if (id.empty() || !util::is_numeric(id))
			{
				throw NotFoundException("Parameters invalid");
			}

			if (multipart.parse(req) != 0)
			{
				throw BadRequestException("Make sure form filled up");
			}

			auto& file = multipart.getFiles()[0];

			util::multipart_tojson(multipart, m_data);

			upload_file upload(&file, std::to_string(trantor::Date::now().microSecondsSinceEpoch()), "products");

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

			this->load_cache();

			if (!m_cache_product.update(stoull(id), product))
				throw NotFoundException("Unable to update non-existing product");

			if (const auto record = db().updateFuture(product).get(); !record)
				throw NotFoundException("Unable to update non-existing product");

			if (multipart.getFiles().size() > 0 && util::allowed_image(file.getFileExtension().data()))
			{
				if (const auto record2 = db_images().updateBy(m_data_image.getMemberNames(),
					Criteria(ProductImages::Cols::_productId, CompareOperator::EQ, id),
					upload.get_image_path(),
					upload.get_thumbnail_path(),
					trantor::Date::now()
				); !record2)
				{
					throw NotFoundException("Unable to update non-existing product");
				}
				upload.save();
			}

			m_response.m_data = product.toJson();
			m_response.m_message = "Success update customer data.";
			m_response.m_success = true;

			return HttpResponse::newHttpJsonResponse(m_response.to_json());
		} EXCEPT_CLAUSE
	}
	HttpResponsePtr product_service::remove(HttpRequestPtr const& req, std::string&& id)
	{
		TRY_CLAUSE
		{
			if (id.empty() || !util::is_numeric(id))
			{
				throw BadRequestException("Parameters requirement doesn't match");
			}

			this->load_cache();

			const auto record = db().deleteFutureByPrimaryKey(stoll(id)).get();

			m_cache_product.remove(stoull(id));

			if (!record)
			{
				throw NotFoundException("Unable to delete non-existing data");
			}

			m_response.m_message = fmt::format("Delete user on {} successfully", record);
			m_response.m_success = true;

			return HttpResponse::newHttpJsonResponse(m_response.to_json());
		} EXCEPT_CLAUSE
	}

	HttpResponsePtr product_service::getProductByCategory(HttpRequestPtr const& req, std::string&& id)
	{
		TRY_CLAUSE
		{
			auto& limitParam = req->getParameter("limit");
			auto& pageParam = req->getParameter("page");

			const size_t limit = limitParam.empty() && !util::is_numeric(limitParam) ? 10 : stoull(limitParam);
			const size_t page = pageParam.empty() && !util::is_numeric(pageParam) ? 0 : stoull(pageParam) - 1;

			if (id.empty() || !util::is_numeric(id))
			{
				throw BadRequestException("Parameters requirement doesn't match");
			}

			this->load_cache();

			auto args = Criteria(MasterProducts::Cols::_categoryId, CompareOperator::EQ, stoull(id));

			const auto products = m_cache_product.find([id](const MasterProducts& entry) -> bool {
				return entry.getValueOfCategoryid() == stoi(id);
			});

			if (products.empty())
			{
				throw NotFoundException("Product which related to that category not found");
			}

			std::ranges::for_each(products.begin(), products.end(), [this](MasterProducts const& product) {
				m_response.m_data.append(product.toJson());
			});

			const size_t lastPage = (products.size() / (limit + (products.size() % limit))) == 0 ? 0 : 1;

			m_response.m_message = "Success retreive products data";
			m_response.m_success = true;
			m_response.m_last_page = lastPage;

			return HttpResponse::newHttpJsonResponse(m_response.to_json());
		}
		EXCEPT_CLAUSE
	}

}