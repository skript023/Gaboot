#pragma once
#include <pch.h>

namespace gaboot
{
	class GabootException : public std::runtime_error
	{
	public:
	 	explicit GabootException(std::string const& message, HttpStatusCode code) : std::runtime_error(message), m_status_code(code), m_message(message) {}
		explicit GabootException(std::string const& message) : std::runtime_error(message) {}
		GabootException() = default;

		virtual ~GabootException() noexcept = default;

		HttpStatusCode get_code() const { return m_status_code; }
		HttpStatusCode get_code() { return m_status_code; }
		HttpResponsePtr response() const
		{
			Json::Value json;
			json["message"] = m_message;
			json["success"] = false;

			auto response = HttpResponse::newHttpJsonResponse(json);
			response->setStatusCode(m_status_code);

			return response;
		}
		
		HttpResponsePtr response()
		{
			Json::Value json;
			json["message"] = m_message;
			json["success"] = false;

			auto response = HttpResponse::newHttpJsonResponse(json);
			response->setStatusCode(m_status_code);

			return response;
		}
	private:
		std::string m_message;
		HttpStatusCode m_status_code;
	};

	template<auto T>
	class CustomException : public GabootException
	{
		std::string m_message;
	public:
		explicit CustomException(std::string const& message) : GabootException(message, T), m_message(message) 
		{
			LOG(WARNING) << m_message;
		}

		virtual ~CustomException() noexcept = default;
	};

	class NotFoundException : public GabootException
	{
		std::string m_message;
	public:
		explicit NotFoundException(std::string const& message = "Unable to retrieve some data, 0 data found.") : GabootException(message, k404NotFound), m_message(message)
		{
			LOG(WARNING) << m_message;
		}

		virtual ~NotFoundException() noexcept = default;
	};

	class BadRequestException : public GabootException
	{
		std::string m_message;
	public:
		explicit BadRequestException(std::string const& message = "Request requirement doesn't match, please check requirement.") : GabootException(message, k400BadRequest), m_message(message)
		{
			LOG(WARNING) << m_message;
		}

		virtual ~BadRequestException() noexcept = default;

		HttpResponsePtr response()
		{
			Json::Value json;
			json["message"] = m_message;
			json["success"] = false;
			auto response = HttpResponse::newHttpJsonResponse(json);
			response->setStatusCode(k400BadRequest);

			return response;
		}
	};

	class UnauthorzedException : public GabootException
	{
		std::string m_message;
	public:
		explicit UnauthorzedException(std::string const& message = "Unauthorized") : GabootException(message, k401Unauthorized), m_message(message) 
		{
			LOG(WARNING) << m_message;
		}
		virtual ~UnauthorzedException() noexcept = default;
	};
}