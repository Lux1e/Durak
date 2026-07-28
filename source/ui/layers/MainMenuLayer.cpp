#include "MainMenuLayer.h"
#include "../elements/RectangleElement.h"
#include "../widgets/Button.h"
#include "../ButtonBuilder.h"
#include "../../core/EventSystem.h"
#include "../../game/events/requests/CreateGameRequestEvent.h"
#include "../../game/events/requests/InputIpAddressRequestEvent.h"
#include "../../game/events/requests/ExitRequestEvent.h"



void MainMenuLayer::init()
{
	m_backgroundPanel = &addPanel(std::make_unique<Panel>(m_size));
	m_mainMenuButtonsPanel = &addPanel(std::make_unique<Panel>());
	m_chooseMenuButtonsPanel = &addPanel(std::make_unique<Panel>(m_size));

	initBackgroundPanel(*m_backgroundPanel);
	initMainMenuButtonsPanel(*m_mainMenuButtonsPanel);
	initChooseMenuButtonsPanel(*m_chooseMenuButtonsPanel);
}


void MainMenuLayer::initBackgroundPanel(Panel& backgroundPanel)
{
	RectangleElement& background = static_cast<RectangleElement&>(backgroundPanel.addChild(std::make_unique<RectangleElement>(backgroundPanel.getSize())));
	background.shape().setFillColor({ 40, 40, 40, 255 });
}

void MainMenuLayer::initMainMenuButtonsPanel(Panel& mainMenuButtonsPanel)
{
	mainMenuButtonsPanel.setSize({ m_size.x / 4.0f, m_size.y / 2.5f });
	mainMenuButtonsPanel.setOrigin({ mainMenuButtonsPanel.getGeometricCenter().x, 0.0f });
	mainMenuButtonsPanel.setPosition({ m_size.x / 2.0f, m_size.y / 2.0f });


	ButtonBuilder buttonBuilder(m_gameContext.textureManager, "PLAY", { mainMenuButtonsPanel.getSize().x, mainMenuButtonsPanel.getSize().y / 4.0f }, { 0.0f, 0.0f }, Fonts::ArialBD);
	Button& playButton = static_cast<Button&>(mainMenuButtonsPanel.addChild(buttonBuilder.build()));

	buttonBuilder.setText("SETTINGS");
	Button& settingsButton = static_cast<Button&>(mainMenuButtonsPanel.addChild(buttonBuilder.build()));

	buttonBuilder.setText("EXIT");
	Button& exitButton = static_cast<Button&>(mainMenuButtonsPanel.addChild(buttonBuilder.build()));


	playButton.setOrigin({ playButton.getGeometricCenter().x, 0.0f });
	settingsButton.setOrigin({ settingsButton.getGeometricCenter() });
	exitButton.setOrigin({ exitButton.getGeometricCenter().x, exitButton.getSize().y });

	playButton.setPosition({ mainMenuButtonsPanel.getSize().x * 0.5f, 0.0f });
	settingsButton.setPosition(mainMenuButtonsPanel.getGeometricCenter());
	exitButton.setPosition({ mainMenuButtonsPanel.getSize().x * 0.5f, mainMenuButtonsPanel.getSize().y });


	playButton.setOnClick([this]()
		{
			m_mainMenuButtonsPanel->setVisible(false);
			m_chooseMenuButtonsPanel->setVisible(true);
		});

	//settingsButton.setOnClick(); to do

	exitButton.setOnClick([this]() { m_gameContext.ES.publish(ExitRequestEvent{}); });
}

void MainMenuLayer::initChooseMenuButtonsPanel(Panel& chooseMenuButtonsPanel)
{
	chooseMenuButtonsPanel.setVisible(false);

	Panel& mainButtonsPanel = static_cast<Panel&>(chooseMenuButtonsPanel.addChild(std::make_unique<Panel>(Panel())));
	mainButtonsPanel.setTransparentToInput(true);
	mainButtonsPanel.setSize({ m_size.x / 3.5f, m_size.y / 5.5f });
	mainButtonsPanel.setOrigin(mainButtonsPanel.getGeometricCenter());
	mainButtonsPanel.setPosition({ m_size.x / 2.0f, m_size.y / 2.0f });


	const sf::Vector2f& mainButtonsPanelSize = mainButtonsPanel.getSize();
	ButtonBuilder buttonBuilder(m_gameContext.textureManager, "create game", { mainButtonsPanelSize.x, mainButtonsPanelSize.y / 2.5f });
	Button& createGameButton = static_cast<Button&>(mainButtonsPanel.addChild(buttonBuilder.build()));

	buttonBuilder.setText("join game");
	Button& joinGameButton = static_cast<Button&>(mainButtonsPanel.addChild(buttonBuilder.build()));


	createGameButton.setOrigin({ createGameButton.getSize().x / 2.0f, 0.0f });
	joinGameButton.setOrigin({ createGameButton.getSize().x / 2.0f, createGameButton.getSize().y });

	createGameButton.setPosition({ mainButtonsPanelSize.x / 2.0f, 0.0f });
	joinGameButton.setPosition({ mainButtonsPanelSize.x / 2.0f, mainButtonsPanelSize.y });


	buttonBuilder.setText("back");
	buttonBuilder.setButtonType(ButtonType::Pilled);
	buttonBuilder.setFont(Fonts::ArialI);
	buttonBuilder.setSize({ chooseMenuButtonsPanel.getSize().x / 12.0f, chooseMenuButtonsPanel.getSize().y / 18.0f });
	Button& backButton = static_cast<Button&>(chooseMenuButtonsPanel.addChild(buttonBuilder.build()));


	backButton.setOrigin(backButton.getGeometricCenter());
	backButton.setPosition({ chooseMenuButtonsPanel.getGeometricCenter().x, chooseMenuButtonsPanel.getSize().y - chooseMenuButtonsPanel.getSize().y * 0.1f });


	createGameButton.setOnClick([this]() { m_gameContext.ES.publish(CreateGameRequestEvent{}); });
	joinGameButton.setOnClick([this]() { m_gameContext.ES.publish(InputIpAddressRequestEvent{}); });
	backButton.setOnClick([this]()
		{
			m_chooseMenuButtonsPanel->setVisible(false);
			m_mainMenuButtonsPanel->setVisible(true);
		});
}


bool MainMenuLayer::handleEvents(const sf::Event& event)
{
	if (const auto& e = event.getIf<sf::Event::MouseButtonPressed>())
	{
		if (e->button == sf::Mouse::Button::Left)
			return onMousePressedEvent();
	}

	if (const auto& e = event.getIf<sf::Event::MouseButtonReleased>())
	{
		if (e->button == sf::Mouse::Button::Left)
			return onMouseReleasedEvent();
	}

	if (const auto& k = event.getIf<sf::Event::KeyReleased>())
	{
		if (k->scancode == sf::Keyboard::Scan::Escape)
		{
			if (m_mainMenuButtonsPanel->isVisible())
			{
				if (m_underMouseElement)
				{
					if (m_underMouseElement->isInteractive())
					{
						auto& interactive = *m_underMouseElement->asInteractive();
						interactive.setHovered(false);
						interactive.setPressed(false, false);
					}
				}

				m_gameContext.ES.publish(ExitRequestEvent{});
			}

			else
			{
				m_mainMenuButtonsPanel->setVisible(true);
				m_chooseMenuButtonsPanel->setVisible(false);
			}

			return true;
		}
	}

	return false;
}