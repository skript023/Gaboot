#pragma once
#include <pch.hpp>

namespace gaboot
{
	class GabootException : public std::runtime_error
	{
	public:

		GabootException(std::string const& message, HttpStatusCode code) : std::runtime_error(message), m_status_code(code) {}
		GabootException(std::string const& message) : std::runtime_error(message) {}
		GabootException() = default;

		~GabootException() noexcept = default;

		HttpStatusCode get_code() const { return m_status_code; }
		HttpStatusCode get_code() { return m_status_code; }
	private:
		HttpStatusCode m_status_code;
	};

	class NotFoundException : public GabootException
	{
	public:
		NotFoundException(std::string const& message) : GabootException(message, k404NotFound)
		{}

		~NotFoundException() noexcept = default;
	};

	class BadRequestException : public GabootException
	{
	public:
		BadRequestException(std::string const& message) : GabootException(message, k400BadRequest)
		{}

		~BadRequestException() noexcept = default;
	};
}