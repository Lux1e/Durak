#include "StringUtils.h"



namespace ui::string
{
	void fitTextToRect(sf::Text& string, const sf::Vector2f textRatio, const sf::FloatRect& rect)
	{
		sf::Vector2f maxAllowedSize = { textRatio.x * rect.size.x, textRatio.y * rect.size.y };

		string.setCharacterSize(static_cast<unsigned int>(maxAllowedSize.y));
		auto textLocalBounds = string.getLocalBounds();

		if (textLocalBounds.size.y > maxAllowedSize.y)
		{
			float ratio = maxAllowedSize.y / textLocalBounds.size.y;
			string.setCharacterSize(static_cast<unsigned int>(string.getCharacterSize() * ratio));
			textLocalBounds = string.getLocalBounds();
		}

		if (textLocalBounds.size.x > maxAllowedSize.x)
		{
			float ratio = maxAllowedSize.x / textLocalBounds.size.x;
			string.setCharacterSize(static_cast<unsigned int>(string.getCharacterSize() * ratio));
			textLocalBounds = string.getLocalBounds();
		}

		string.setOrigin({ textLocalBounds.position.x + textLocalBounds.size.x / 2, textLocalBounds.position.y + textLocalBounds.size.y / 2 });
		string.setPosition({ rect.position.x + rect.size.x / 2.0f, rect.position.y + rect.size.y / 2.0f });
	}
}