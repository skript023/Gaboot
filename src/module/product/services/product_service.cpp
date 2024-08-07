#include "product_service.hpp"
#include "uploader/upload.hpp"
#include "exception/exception.hpp"

namespace gaboot
{
	product_service::product_service():
		m_product(std::make_unique<Mapper<MasterProducts>>(DATABASE_CLIENT)),
		m_product_image(std::make_unique<Mapper<ProductImages>>(DATABASE_CLIENT)),
		m_response(std::make_unique<response_data<ProductResponse>>())
	{
		LOG(INFO) << "Product service registered.";
	}
	product_service::~product_service() noexcept
	{
		m_product.reset();
		m_product_image.reset();
		m_response.reset();
	}
	HttpResponsePtr product_service::findAll(HttpRequestPtr const& req)
	{
		auto& limitParam = req->getParameter("limit");
		auto& pageParam = req->getParameter("page");
		auto& categoryId = req->getParameter("categoryId");

		const size_t limit = limitParam.empty() && !util::is_numeric(limitParam) ? 10 : stoull(limitParam);
		const size_t page = pageParam.empty() && !util::is_numeric(pageParam) ? 0 : stoull(pageParam) - 1;

		const auto products = categoryId.empty() ? m_product->limit(limit).offset(page * limit).findAll() : m_product->findBy(Criteria(MasterProducts::Cols::_category_id, CompareOperator::EQ, categoryId));

		if (products.empty())
		{
			m_response->m_message = "Product data is empty 0 data found";
			m_response->m_success = true;

			return m_response->json();
		}

		const size_t lastPage = (products.size() / (limit + (products.size() % limit))) == 0 ? 0 : 1;

		m_response->m_message = "Success retreive products data";
		m_response->m_success = true;
		m_response->m_data = products;
		m_response->m_last_page = lastPage;

		return m_response->json();
	}
	HttpResponsePtr product_service::create(HttpRequestPtr const& req)
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
		productImage.setImagePath(upload.get_image_path());
		productImage.setThumbnailPath(upload.get_thumbnail_path());

		productImage.setCreatedAt(trantor::Date::now());
		productImage.setUpdatedAt(trantor::Date::now());

		product.setPrice(m_data["price"].asDouble());
		product.setCreatedAt(trantor::Date::now());
		product.setUpdatedAt(trantor::Date::now());

		if (!schema.validate(product.toJson(), m_error))
		{
			throw BadRequestException(m_error);
		}

		m_product->insert(product);
		productImage.setProductId(product.getPrimaryKey());
		m_product_image->insert(productImage);

		m_cache_product.clear();

		upload.save();

		m_response->m_message = "Create product success";
		m_response->m_success = true;

		return m_response->status(k201Created)->json();
	}
	HttpResponsePtr product_service::findOne(HttpRequestPtr const& req, std::string&& id)
	{
		if (id.empty())
		{
			throw BadRequestException("Parameters requirement doesn't match");
		}

		auto product = m_product->findByPrimaryKey(id);

		m_response->m_message = "Success retrieve products data";
		m_response->m_data = product;
		m_response->m_success = true;

		return m_response->json();
	}
	HttpResponsePtr product_service::update(HttpRequestPtr const& req, std::string&& id)
	{
		MultiPartParser multipart;

		if (id.empty())
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

		m_data["updated_at"] = trantor::Date::now().toDbStringLocal();

		if (multipart.getFiles().size() > 0 && util::allowed_image(file.getFileExtension().data()))
		{
			m_data_image["image_path"] = upload.get_image_path();
			m_data_image["thumbnail_path"] = upload.get_thumbnail_path();
			m_data_image["updated_at"] = trantor::Date::now().toDbStringLocal();
		}

		MasterProducts product(m_data);
		product.setId(id);
		product.setUpdatedAt(trantor::Date::now());

		if (const auto record = m_product->updateFuture(product).get(); !record)
			throw NotFoundException("Unable to update non-existing product");

		if (multipart.getFiles().size() > 0 && util::allowed_image(file.getFileExtension().data()))
		{
			if (const auto record2 = m_product_image->updateBy(m_data_image.getMemberNames(),
				Criteria(ProductImages::Cols::_product_id, CompareOperator::EQ, id),
				upload.get_image_path(),
				upload.get_thumbnail_path(),
				trantor::Date::now()
			); !record2)
			{
				throw NotFoundException("Unable to update non-existing product");
			}
			upload.save();
		}

		m_response->m_message = "Success update customer data.";
		m_response->m_success = true;

		return m_response->json();
	}
	HttpResponsePtr product_service::remove(HttpRequestPtr const& req, std::string&& id)
	{
		if (id.empty())
		{
			throw BadRequestException("Parameters requirement doesn't match");
		}

		const auto record = m_product->deleteByPrimaryKey(id);

		if (!record)
		{
			throw NotFoundException("Unable to delete non-existing data");
		}

		m_response->m_message = fmt::format("Delete user on {} successfully", record);
		m_response->m_success = true;

		return m_response->json();
	}

	HttpResponsePtr product_service::getProductByCategory(HttpRequestPtr const& req, std::string&& id)
	{
		auto& limitParam = req->getParameter("limit");
		auto& pageParam = req->getParameter("page");

		const size_t limit = limitParam.empty() && !util::is_numeric(limitParam) ? 10 : stoull(limitParam);
		const size_t page = pageParam.empty() && !util::is_numeric(pageParam) ? 0 : stoull(pageParam) - 1;

		if (id.empty())
		{
			throw BadRequestException("Parameters requirement doesn't match");
		}

		this->load_cache();

		auto args = Criteria(MasterProducts::Cols::_category_id, CompareOperator::EQ, id);

		const auto products = m_product->findBy(args);

		if (products.empty())
		{
			throw NotFoundException("Product which related to that category not found");
		}

		const size_t lastPage = (products.size() / (limit + (products.size() % limit))) == 0 ? 0 : 1;

		m_response->m_data = products;
		m_response->m_message = "Success retreive products data";
		m_response->m_success = true;
		m_response->m_last_page = lastPage;

		return m_response->json();
	}

	HttpResponsePtr product_service::getProductWithImage(HttpRequestPtr const& req)
	{
		auto& limitParam = req->getParameter("limit");
		auto& pageParam = req->getParameter("page");

		const size_t limit = limitParam.empty() && !util::is_numeric(limitParam) ? 10 : stoull(limitParam);
		const size_t page = pageParam.empty() && !util::is_numeric(pageParam) ? 0 : stoull(pageParam) - 1;

		const auto products = m_product->findAll();

		const size_t lastPage = (products.size() / (limit + (products.size() % limit))) == 0 ? 0 : 1;

		std::ranges::for_each(products.begin(), products.end(), [this](MasterProducts product) {
			auto images = product.getProduct_images(DATABASE_CLIENT);
			m_response->m_data.push(images);
		});
		m_response->m_message = "Success retreive products data";
		m_response->m_data = products;
		m_response->m_success = true;
		m_response->m_last_page = lastPage;

		return m_response->json();
	}
}