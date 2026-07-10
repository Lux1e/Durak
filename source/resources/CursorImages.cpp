#include "CursorsImages.h"



namespace CursorsImages
{
	std::optional<sf::Cursor> GradientCursor;
	sf::Image GradientImage;
	const sf::Vector2u GradientHotSpot{ 1, 1 };

	void loadImages()
	{
		if (!GradientImage.loadFromFile("assets/images/Gradient32x32.png"))
			throw std::runtime_error("Failed to load GradientCursor");

		GradientCursor = sf::Cursor::createFromPixels(GradientImage.getPixelsPtr(), GradientImage.getSize(), GradientHotSpot);
	}
}