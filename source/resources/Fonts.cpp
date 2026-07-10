#include "Fonts.h"



namespace Fonts
{
	sf::Font Arial;
	sf::Font ArialBD;
	sf::Font ArialI;
	sf::Font ArialBI;

	void loadFonts()
	{
		if (!Arial.openFromFile("assets/fonts/arial.ttf"))
			throw std::runtime_error("Failed to load font: Arial");
		
		if (!ArialBD.openFromFile("assets/fonts/arialbd.ttf"))
			throw std::runtime_error("Failed to load font: ArialBD");

		if (!ArialI.openFromFile("assets/fonts/ariali.ttf"))
			throw std::runtime_error("Failed to load font: ArialI");

		if (!ArialBI.openFromFile("assets/fonts/arialbi.ttf"))
			throw std::runtime_error("Failed to load font: ArialBI");
	}
}