#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>
#include <SFML/Window/ContextSettings.hpp>
#include "core/LaunchArgs.h"
#include "game/Game.h"
#include "resources/Fonts.h"
#include "resources/CursorsImages.h"
#include "ui/widgets/ListBox.h"
#include "ui/widgets/TextBox.h"



int main(int argc, char* argv[])
{
	LaunchArgs args = parseArgs(argc, argv);

	Fonts::loadFonts();
	CursorsImages::loadImages();

	sf::ContextSettings settings;
	settings.antiAliasingLevel = 8;

	sf::RenderWindow window(sf::VideoMode({ 960, 540 }), "Durak Online", sf::Style::Default, sf::State::Windowed, settings);
	//sf::RenderWindow window(sf::VideoMode({ 1920, 1080 }), "Durak Online", sf::Style::Default, sf::State::Windowed, settings);
	//sf::RenderWindow window(sf::VideoMode::getDesktopMode(), "Durak Online", sf::Style::Default, sf::State::Fullscreen, settings);
	//sf::RenderWindow window(sf::VideoMode::getDesktopMode(), "Durak Online", sf::Style::None, sf::State::Windowed, settings);

	window.setMouseCursor(CursorsImages::GradientCursor.value());

	window.setFramerateLimit(300);
	sf::Clock deltaClock;

	Game game(args.profileIndex, window); 

	/////////////////////////////////////////////////////////////////////////////////
	auto& textures = game.getTextureManager();
	/////////////////////////////////////////////////////////////////////////////////


	/////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////


	while (window.isOpen())
	{
		float deltaTime = deltaClock.restart().asSeconds();

		game.processReceivingPacket();
		game.updateMouseInput();

		while (const std::optional event = window.pollEvent())
		{
			if (event->is<sf::Event::Closed>())
				window.close();

			game.handleEvents(*event);
		}

		game.dispatchEvents();
		game.update(deltaTime);
		game.defer();
		game.cleanUp();
		game.processSendingPackets();

		window.clear();
		game.render();
		window.display();
	}

	return 0;
}