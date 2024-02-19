#pragma once

#include <cstddef>
#include <cstdint>
#include <string_view>
#include <type_traits>

namespace jenkins
{
	using Hash = std::uint32_t;

	inline constexpr char string_to_lower(char c)
	{
		return (c >= 'A' && c <= 'Z') ? c + ('a' - 'A') : c;
	}

	template <std::size_t CharCount>
	struct constexpr_hash
	{
		char data[CharCount];

		template <std::size_t... Indices>
		constexpr constexpr_hash(const char *str, std::index_sequence<Indices...>) :
			data{ (str[Indices])... }
		{
		}

		constexpr Hash operator()()
		{
			Hash hash = 0;

			for (std::size_t i = 0; i < CharCount; ++i)
			{
				hash += string_to_lower(data[i]);
				hash += (hash << 10);
				hash ^= (hash >> 6);
			}

			hash += (hash << 3);
			hash ^= (hash >> 11);
			hash += (hash << 15);

			return hash;
		}
	};

	inline Hash hash(std::string_view str)
	{
		Hash hash = 0;

		for (char c : str)
		{
			hash += string_to_lower(c);
			hash += (hash << 10);
			hash ^= (hash >> 6);
		}

		hash += (hash << 3);
		hash ^= (hash >> 11);
		hash += (hash << 15);

		return hash;
	}

	inline Hash hash(const char *str)
	{
		Hash hash = 0;

		while (*str)
		{
			hash += string_to_lower(*(str++));
			hash += (hash << 10);
			hash ^= (hash >> 6);
		}

		hash += (hash << 3);
		hash ^= (hash >> 11);
		hash += (hash << 15);

		return hash;
	}
}

#define JENKINS_HASH_IMPL(str) (::jenkins::constexpr_hash<sizeof(str) - 1>((str), std::make_index_sequence<sizeof(str) - 1>())())
#define JENKINS_HASH(str) (std::integral_constant<jenkins::Hash, JENKINS_HASH_IMPL(str)>::value)