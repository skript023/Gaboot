#pragma once
#include <pch.h>

using namespace drogon;
using namespace orm;
using namespace drogon_model::gaboot;

namespace gaboot
{
	struct CustomerResponse
	{
		CustomerResponse() = default;

		CustomerResponse(MasterCustomers* res):
			id(res->getValueOfId()), 
			firstname(res->getValueOfFirstname()),
			lastname(res->getValueOfLastname()),
			username(res->getValueOfUsername()),
			email(res->getValueOfEmail()),
			phoneNumber(res->getValueOfPhoneNumber()),
			addressDetail(res->getValueOfAddressDetail()),
			latitude(res->getValueOfLatitude()),
			longitude(res->getValueOfLongitude()),
			password(res->getValueOfPassword()),
			token(res->getValueOfToken()),
			isActive(res->getValueOfIsActive()),
			imagePath(res->getValueOfImagePath()),
			thumbnailPath(res->getValueOfThumbnailPath()),
			createdAt(res->getValueOfCreatedAt().toDbStringLocal()),
			updatedAt(res->getValueOfUpdatedAt().toDbStringLocal())
		{

		}
		
		CustomerResponse(MasterCustomers const& res):
			id(res.getValueOfId()), 
			firstname(res.getValueOfFirstname()),
			lastname(res.getValueOfLastname()),
			username(res.getValueOfUsername()),
			email(res.getValueOfEmail()),
			phoneNumber(res.getValueOfPhoneNumber()),
			addressDetail(res.getValueOfAddressDetail()),
			latitude(res.getValueOfLatitude()),
			longitude(res.getValueOfLongitude()),
			password(res.getValueOfPassword()),
			token(res.getValueOfToken()),
			isActive(res.getValueOfIsActive()),
			imagePath(res.getValueOfImagePath()),
			thumbnailPath(res.getValueOfThumbnailPath()),
			createdAt(res.getValueOfCreatedAt().toDbStringLocal()),
			updatedAt(res.getValueOfUpdatedAt().toDbStringLocal())
		{

		}

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

		Json::Value to_json()
		{
			nlohmann::json json = *this;
			Json::Value data;
			Json::Reader reader;
			
			reader.parse(json.dump(), data);

			return data;
		}
		NLOHMANN_DEFINE_TYPE_INTRUSIVE(CustomerResponse, id, firstname, lastname, username, email, phoneNumber, addressDetail, latitude, longitude, password, token, isActive, imagePath, thumbnailPath, createdAt, updatedAt)
	};
}