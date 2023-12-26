#pragma once

namespace gaboot
{
	class money_formatter
	{
	public:
		money_formatter(double money) :
			m_money(money),
			m_format("ID"),
			output(std::use_facet<std::money_put<char>>(m_format)),
			iterator(os)
		{
			os.imbue(m_format);
			os.setf(std::ios_base::showbase);
		}
		
		money_formatter(double money, std::string format) :
			m_money(money),
			m_format(std::move(format)),
			output(std::use_facet<std::money_put<char>>(m_format)),
			iterator(os)
		{
			os.imbue(m_format);
			os.setf(std::ios_base::showbase);
		}

		~money_formatter() noexcept = default;

		std::string get()
		{
			os.str("");
			output.put(iterator, false, os, ' ', m_money * 100);

			return os.str();
		}

		std::string as_string(float value)
		{
			os.str("");  // clear string
			if ((m_money < static_cast<float>(std::numeric_limits<int>::max())) && (m_money > static_cast<float>(std::numeric_limits<int>::min())))
			{
				output.put(iterator, false, os, ' ', value * 100);
			}
			else
			{
				output.put(iterator, false, os, ' ', value);
			}
			return os.str();
		}

		std::string as_string(double value)
		{
			os.str("");  // clear string
			output.put(iterator, false, os, ' ', value);

			return os.str();
		}

	private:
		std::locale m_format;
		const std::money_put<char>& output;
		std::ostringstream os;
		std::ostreambuf_iterator<char, std::char_traits<char> > iterator;
		double m_money;
	};
}