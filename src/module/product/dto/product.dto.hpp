#pragma once
#include <pch.h>
#include <product/models/MasterProducts.h>
#include <product/models/ProductImages.h>

using namespace drogon;
using namespace orm;
using namespace drogon_model::gaboot;

namespace gaboot
{
	struct ProductResponse
	{
		struct ProductImageResponse
		{
			ProductImageResponse operator=(ProductImages const& res)
			{
				id = res.getValueOfId();
				imagePath = res.getValueOfImagePath();
				thumbnailPath = res.getValueOfThumbnailPath();
				productId = res.getValueOfProductId();
				isCover = res.getValueOfIsCover();
				createdAt = res.getValueOfCreatedAt().toDbStringLocal();
				updatedAt = res.getValueOfUpdatedAt().toDbStringLocal();

				return *this;
			}

			std::string id;
			std::string imagePath;
			std::string thumbnailPath;
			std::string productId;
			bool isCover;
			std::string updatedAt;
			std::string createdAt;

			Json::Value to_json()
			{
				nlohmann::json json = *this;

				Json::Value data;
				Json::Reader reader;

				reader.parse(json.dump(), data);

				return data;
			}

			NLOHMANN_DEFINE_TYPE_INTRUSIVE(ProductImageResponse, id, imagePath, thumbnailPath, productId, isCover, updatedAt, createdAt)
		};

		ProductResponse() = default;

		ProductResponse(std::vector<MasterProducts> products, std::vector<ProductResponse> response)
		{
			std::ranges::for_each(products.begin(), products.end(), [this](MasterProducts const& res) {
				id = res.getValueOfId();
				name = res.getValueOfName();
				description = res.getValueOfDescription();
				price = res.getValueOfPrice();
				stock = res.getValueOfStock();
				dimension = res.getValueOfDimension();
				weight = res.getValueOfWeight();
				weightUnit = res.getValueOfWeightUnit();
				categoryId = res.getValueOfCategoryId();
				totalSales = res.getValueOfTotalSales();
				isActive = res.getValueOfIsActive();
				createdAt = res.getValueOfCreatedAt().toDbStringLocal();
				updatedAt = res.getValueOfUpdatedAt().toDbStringLocal();
			});
		}
		
		ProductResponse(MasterProducts* res) :
			id(res->getValueOfId()),
			name(res->getValueOfName()),
			description(res->getValueOfDescription()),
			price(res->getValueOfPrice()),
			stock(res->getValueOfStock()),
			dimension(res->getValueOfDimension()),
			weight(res->getValueOfWeight()),
			weightUnit(res->getValueOfWeightUnit()),
			categoryId(res->getValueOfCategoryId()),
			totalSales(res->getValueOfTotalSales()),
			isActive(res->getValueOfIsActive()),
			createdAt(res->getValueOfCreatedAt().toDbStringLocal()),
			updatedAt(res->getValueOfUpdatedAt().toDbStringLocal())
		{
			//if (!res) throw NotFoundException("Product data is empty 0 data found");
		}

		ProductResponse(MasterProducts res):
			id(res.getValueOfId()), 
			name(res.getValueOfName()),
			description(res.getValueOfDescription()),
			price(res.getValueOfPrice()),
			stock(res.getValueOfStock()),
			dimension(res.getValueOfDimension()),
			weight(res.getValueOfWeight()),
			weightUnit(res.getValueOfWeightUnit()),
			categoryId(res.getValueOfCategoryId()),
			totalSales(res.getValueOfTotalSales()),
			isActive(res.getValueOfIsActive()),
			createdAt(res.getValueOfCreatedAt().toDbStringLocal()),
			updatedAt(res.getValueOfUpdatedAt().toDbStringLocal())
		{

		}

		std::string id;
		std::string name;
		std::string description;
		double price;
		int32_t stock;
		std::string dimension;
		double weight;
		std::string weightUnit;
		std::string categoryId;
		int32_t totalSales;
		bool isActive;
		std::string createdAt;
		std::string updatedAt;
		std::vector<ProductImageResponse> images;

		void push(ProductImages image)
		{
			ProductResponse::ProductImageResponse res;
			res.id = image.getValueOfId();
			res.imagePath = image.getValueOfImagePath();
			res.thumbnailPath = image.getValueOfThumbnailPath();
			res.productId = image.getValueOfProductId();
			res.isCover = image.getValueOfIsCover();
			res.createdAt = image.getValueOfCreatedAt().toDbStringLocal();
			res.updatedAt = image.getValueOfUpdatedAt().toDbStringLocal();

			images.push_back(res);
		}
		
		void push(std::vector<ProductImages> images)
		{
			std::ranges::for_each(images.begin(), images.end(), [this](ProductImages const& image) {
				ProductImageResponse res;
				res.id = image.getValueOfId();
				res.imagePath = image.getValueOfImagePath();
				res.thumbnailPath = image.getValueOfThumbnailPath();
				res.productId = image.getValueOfProductId();
				res.isCover = image.getValueOfIsCover();
				res.createdAt = image.getValueOfCreatedAt().toDbStringLocal();
				res.updatedAt = image.getValueOfUpdatedAt().toDbStringLocal();

				this->images.push_back(res);
			});
		}

		Json::Value to_json()
		{
			nlohmann::json json = *this;

			if (images.empty()) json.erase("images");

			Json::Value data;
			Json::Reader reader;
			
			reader.parse(json.dump(), data);

			return data;
		}
		NLOHMANN_DEFINE_TYPE_INTRUSIVE(ProductResponse, id, name, description, price, stock, dimension, weight, weightUnit, categoryId, totalSales, isActive, createdAt, updatedAt, images)
	};
}