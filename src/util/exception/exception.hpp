#pragma once
#include <pch.h>
#include <benchmark.hpp>

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
		explicit NotFoundException(std::string const& message = "Not Found") : GabootException(message, k404NotFound), m_message(message)
		{
			LOG(WARNING) << m_message;
		}

		virtual ~NotFoundException() noexcept = default;
	};

	class BadRequestException : public GabootException
	{
		std::string m_message;
	public:
		explicit BadRequestException(std::string const& message = "Bad Request") : GabootException(message, k400BadRequest), m_message(message)
		{
			LOG(WARNING) << m_message;
		}

		virtual ~BadRequestException() noexcept = default;
	};

	class UnauthorizedException : public GabootException
	{
		std::string m_message;
	public:
		explicit UnauthorizedException(std::string const& message = "Unauthorized") : GabootException(message, k401Unauthorized), m_message(message)
		{
			LOG(WARNING) << m_message;
		}

		virtual ~UnauthorizedException() noexcept = default;
	};
	
	class InternalServerErrorException : public GabootException
	{
		std::string m_message;
	public:
		explicit InternalServerErrorException(std::string const& message = "Internal Server Error") : GabootException(message, k500InternalServerError), m_message(message)
		{
			LOG(WARNING) << m_message;
		}

		virtual ~InternalServerErrorException() noexcept = default;
	};

#define TRY_CLAUSE_OLD \
	benchmark bench; \
	try {
#define EXCEPT_CLAUSE_OLD \
	bench.get_runtime(); \
	bench.reset(); \
	} \
	catch (GabootException const& ex) { return ex.response(); } \
	catch(DrogonDbException const& ex) { return CustomException<k500InternalServerError>(fmt::format("error caught on {}", ex.base().what())).response(); } \
	catch(std::exception const& ex) { return CustomException<k500InternalServerError>(fmt::format("error caught on {}", ex.what())).response(); }
#define TRY_CLAUSE \
    try { \
        benchmark::execute([&]{

#define EXCEPT_CLAUSE \
        }); \
    } \
    catch (GabootException const& ex) { return callback(ex.response()); } \
    catch(DrogonDbException const& ex) { return callback(CustomException<k500InternalServerError>(fmt::format("error caught on {}", ex.base().what())).response()); } \
    catch(std::exception const& ex) { return callback(CustomException<k500InternalServerError>(fmt::format("error caught on {}", ex.what())).response()); }
}