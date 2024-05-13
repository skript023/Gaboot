#pragma once
#include <pch.h>

using namespace drogon;
using namespace orm;
using namespace drogon_model::gaboot;

namespace gaboot
{
	struct CustomerResponse
	{
		std::string id;
		std::string firstname;
		std::string lastname;
		std::string username;
		std::string email;
		std::string phoneNumber;
		std::string addressDetail;
		double latitude;
		double longitude;
		std::string password;
		std::string token;
		bool isActive;
		std::string imagePath;
		std::string thumbnailPath;
		std::string createdAt;
		std::string updatedAt;
		std::vector<CustomerResponse> m_vector;

		void clear()
		{
			id.clear();
			firstname.clear();
			lastname.clear();
			username.clear();
			email.clear();
			phoneNumber.clear();
			addressDetail.clear();
			latitude = 0.0;
			longitude = 0.0;
			password.clear();
			token.clear();
			isActive = false;
			imagePath.clear();
			thumbnailPath.clear();
			createdAt.clear();
			thumbnailPath.clear();
			createdAt.clear();
			updatedAt.clear();
			m_vector.clear();
		}

		bool empty()
		{
			return id.empty() && firstname.empty() && lastname.empty() && username.empty() &&
				email.empty() && phoneNumber.empty() && addressDetail.empty() &&
				latitude == 0.0 && longitude == 0.0 && password.empty() &&
				token.empty() && !isActive && imagePath.empty() &&
				thumbnailPath.empty() && createdAt.empty() && updatedAt.empty();
		}

		Json::Value to_json()
		{
			nlohmann::json json;

			if (m_vector.empty())
			{
				json = *this;
			}
			else
			{
				for (auto& var : m_vector)
				{
					json.emplace_back(var);
				}
			}
			Json::Value data;
			Json::Reader reader;
			
			reader.parse(json.dump(), data);

			if (!m_vector.empty()) m_vector.clear();

			return data;
		}

		template<typename U>
		typename std::enable_if<std::is_same<U, std::vector<MasterCustomers>>::value, void>::type operator=(const U& args)
		{
			for (const auto& res : args)
			{
				id = res.getValueOfId();
				firstname = res.getValueOfFirstname();
				lastname = res.getValueOfLastname();
				username = res.getValueOfUsername();
				email = res.getValueOfEmail();
				phoneNumber = res.getValueOfPhoneNumber();
				addressDetail = res.getValueOfAddressDetail();
				latitude = res.getValueOfLatitude();
				longitude = res.getValueOfLongitude();
				password = res.getValueOfPassword();
				token = res.getValueOfToken();
				isActive = res.getValueOfIsActive();
				imagePath = res.getValueOfImagePath();
				thumbnailPath = res.getValueOfThumbnailPath();
				createdAt = res.getValueOfCreatedAt().toDbStringLocal();
				updatedAt = res.getValueOfUpdatedAt().toDbStringLocal();

				m_vector.emplace_back(*this);
			}
		}

		template<typename U>
		typename std::enable_if<std::is_same<U, MasterCustomers>::value, void>::type operator=(const U& args)
		{
			id = args.getValueOfId();
			firstname = args.getValueOfFirstname();
			lastname = args.getValueOfLastname();
			username = args.getValueOfUsername();
			email = args.getValueOfEmail();
			phoneNumber = args.getValueOfPhoneNumber();
			addressDetail = args.getValueOfAddressDetail();
			latitude = args.getValueOfLatitude();
			longitude = args.getValueOfLongitude();
			password = args.getValueOfPassword();
			token = args.getValueOfToken();
			isActive = args.getValueOfIsActive();
			imagePath = args.getValueOfImagePath();
			thumbnailPath = args.getValueOfThumbnailPath();
			createdAt = args.getValueOfCreatedAt().toDbStringLocal();
			updatedAt = args.getValueOfUpdatedAt().toDbStringLocal();
		}

		template<typename U>
		typename std::enable_if<std::is_same<U, MasterCustomers*>::value, void>::type operator=(U args)
		{
			id = args->getValueOfId();
			firstname = args->getValueOfFirstname();
			lastname = args->getValueOfLastname();
			username = args->getValueOfUsername();
			email = args->getValueOfEmail();
			phoneNumber = args->getValueOfPhoneNumber();
			addressDetail = args->getValueOfAddressDetail();
			latitude = args->getValueOfLatitude();
			longitude = args->getValueOfLongitude();
			password = args->getValueOfPassword();
			token = args->getValueOfToken();
			isActive = args->getValueOfIsActive();
			imagePath = args->getValueOfImagePath();
			thumbnailPath = args->getValueOfThumbnailPath();
			createdAt = args->getValueOfCreatedAt().toDbStringLocal();
			updatedAt = args->getValueOfUpdatedAt().toDbStringLocal();
		}

		NLOHMANN_DEFINE_TYPE_INTRUSIVE(CustomerResponse, id, firstname, lastname, username, email, phoneNumber, addressDetail, latitude, longitude, password, token, isActive, imagePath, thumbnailPath, createdAt, updatedAt)
	};
}