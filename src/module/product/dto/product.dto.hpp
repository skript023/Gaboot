#pragma once
#include <pch.h>
#include <product/models/MasterProducts.h>
#include <product/models/ProductImages.h>

using namespace drogon;
using namespace orm;
using namespace drogon_model::gaboot;

namespace gaboot
{
	struct ProductImageResponse
	{
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

		template<typename U>
		std::enable_if<std::is_same<U, std::vector<ProductImages>>::value, void>::type operator=(const U& res)
		{
			id = res.getValueOfId();
			imagePath = res.getValueOfImagePath();
			thumbnailPath = res.getValueOfThumbnailPath();
			productId = res.getValueOfProductId();
			isCover = res.getValueOfIsCover();
			createdAt = res.getValueOfCreatedAt().toDbStringLocal();
			updatedAt = res.getValueOfUpdatedAt().toDbStringLocal();
		}

		template<typename U>
		std::enable_if<std::is_same<U, ProductImages>::value, void>::type operator=(const U& res)
		{
			id = res.getValueOfId();
			imagePath = res.getValueOfImagePath();
			thumbnailPath = res.getValueOfThumbnailPath();
			productId = res.getValueOfProductId();
			isCover = res.getValueOfIsCover();
			createdAt = res.getValueOfCreatedAt().toDbStringLocal();
			updatedAt = res.getValueOfUpdatedAt().toDbStringLocal();
		}

		template<typename U>
		std::enable_if<std::is_same<U, ProductImages*>::value, void>::type operator=(const U& res)
		{
			id = res->getValueOfId();
			imagePath = res->getValueOfImagePath();
			thumbnailPath = res->getValueOfThumbnailPath();
			productId = res->getValueOfProductId();
			isCover = res->getValueOfIsCover();
			createdAt = res->getValueOfCreatedAt().toDbStringLocal();
			updatedAt = res->getValueOfUpdatedAt().toDbStringLocal();
		}

		NLOHMANN_DEFINE_TYPE_INTRUSIVE(ProductImageResponse, id, imagePath, thumbnailPath, productId, isCover, updatedAt, createdAt)
	};

	struct ProductResponse
	{
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
		std::vector<ProductResponse> m_vector;

		void push(ProductImages image)
		{
			ProductImageResponse res;
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
			nlohmann::json json;

			if (m_vector.empty())
			{
				json = *this;

				if (images.empty()) json.erase("images");
			}
			else
			{
				for (auto it = m_vector.begin(); it != m_vector.end(); ++it)
				{
					nlohmann::json j = *it;

					if (images.empty()) j.erase("images");

					json.emplace_back(j);
				}
			}

			Json::Value data;
			Json::Reader reader;
			
			reader.parse(json.dump(), data);

			return data;
		}

		template<typename U>
		std::enable_if<std::is_same<U, std::vector<MasterProducts>>::value, void>::type operator=(const U& args)
		{
			for (const auto& res : args)
			{
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

				m_vector.emplace_back(*this);
			}
		}

		template<typename U>
		std::enable_if<std::is_same<U, MasterProducts>::value, void>::type operator=(const U& args)
		{
			id = args.getValueOfId();
			name = args.getValueOfName();
			description = args.getValueOfDescription();
			price = args.getValueOfPrice();
			stock = args.getValueOfStock();
			dimension = args.getValueOfDimension();
			weight = args.getValueOfWeight();
			weightUnit = args.getValueOfWeightUnit();
			categoryId = args.getValueOfCategoryId();
			totalSales = args.getValueOfTotalSales();
			isActive = args.getValueOfIsActive();
			createdAt = args.getValueOfCreatedAt().toDbStringLocal();
			updatedAt = args.getValueOfUpdatedAt().toDbStringLocal();
		}

		template<typename U>
		std::enable_if<std::is_same<U, MasterProducts*>::value, void>::type operator=(U args)
		{
			id = args->getValueOfId();
			name = args->getValueOfName();
			description = args->getValueOfDescription();
			price = args->getValueOfPrice();
			stock = args->getValueOfStock();
			dimension = args->getValueOfDimension();
			weight = args->getValueOfWeight();
			weightUnit = args->getValueOfWeightUnit();
			categoryId = args->getValueOfCategoryId();
			totalSales = args->getValueOfTotalSales();
			isActive = args->getValueOfIsActive();
			createdAt = args->getValueOfCreatedAt().toDbStringLocal();
			updatedAt = args->getValueOfUpdatedAt().toDbStringLocal();
		}

		NLOHMANN_DEFINE_TYPE_INTRUSIVE(ProductResponse, id, name, description, price, stock, dimension, weight, weightUnit, categoryId, totalSales, isActive, createdAt, updatedAt, images)
	};
}