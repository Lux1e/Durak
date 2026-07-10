#include "NetworkUtils.h"



namespace NetworkUtils
{
	bool isValidIP(const std::string& string)
	{
		const std::string& currentIP = string;

		int parts = 1;
		int digits = 0;
		int value = 0;

		size_t symbolsSize = currentIP.size();

		if (symbolsSize < 7 || symbolsSize > 15 || !std::isdigit(currentIP[symbolsSize - 1]))
			return false;

		for (size_t i = 0; i < symbolsSize; ++i)
		{
			const char& symbol = currentIP[i];

			if (symbol == '.')
			{
				if (digits == 0 || parts >= 4)
					return false;

				++parts;
				digits = 0;
				value = 0;
			}

			else if (std::isdigit(symbol))
			{
				++digits;

				if (digits > 3)
					return false;

				value = value * 10 + (symbol - '0');

				if (value > 255)
					return false;
			}

			else
				return false;
		}

		return parts == 4;
	}
}