#pragma once
#include <pch.h>

namespace gaboot
{
	struct ActualCategoryRespose
	{
		std::string id;
		std::string firstname;
		std::string lastname;
		std::string username;
		std::string email;
		std::string phone_number;
		std::string address_detail;
		double latitude;
		double longitude;
		std::string password;
		std::string token;
		bool is_active;
		std::string image_path;
		std::string thumbnail_path;
		std::string created_at;
		std::string updated_at;

		NLOHMANN_DEFINE_TYPE_INTRUSIVE(ActualCustomerResponse, id, firstname, lastname, username, email, phone_number, address_detail, latitude, longitude, password, token, is_active, image_path, thumbnail_path, created_at, updated_at)
	};

	struct CustomerResponse
	{
		CustomerResponse() = default;

		CustomerResponse(std::unique_ptr<ActualCustomerResponse> const& res):
			id(res->id), 
			firstname(res->firstname),
			lastname(res->lastname),
			username(res->username),
			email(res->email),
			phoneNumber(res->phone_number),
			addressDetail(res->address_detail),
			latitude(res->latitude),
			longitude(res->longitude),
			password(res->password),
			token(res->token),
			isactive(res->is_active),
			imagepath(res->image_path),
			thumbnailPath(res->thumbnail_path),
			createdAt(res->created_at),
			updatedAt(res->updated_at)
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
		std::string isActive;
		std::string imagePath;
		std::string thumbnailPath;
		std::string createdAt;
		std::string updatedAt;

		CustomerResponse from_json(Json::Value const& json)
		{
			auto njson = nlohmann::json::parse(json.toStyledString());

			const auto response = std::make_unique<ActualCustomerResponse>(njson.get<ActualCustomerResponse>());

			*this = response;

            return *this;
		}

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