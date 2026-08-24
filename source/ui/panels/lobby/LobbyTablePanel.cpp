#include "LobbyTablePanel.h"
#include "LobbySeatsPanel.h"
#include "../../../core/EventSystem.h"
#include "../../../core/ClientContext.h"
#include "../../../resources/TextureManager.h"
#include "../../../game/GameContext.h"
#include "../../../states/client/ClientLobbyState.h"
#include "../../elements/RoundedRectangleElement.h"
#include "../../elements/SpriteElement.h"
#include "../../widgets/Button.h"
#include "../../styles/VisualComponent.h"
#include "../../../game/events/requests/PlayersPerGameChangeRequestEvent.h"



void LobbyTablePanel::init(const ClientLobbyState& clientLobbyState, const GameContext& gameContext, const ClientContext& clientContext, const sf::Vector2f& lobbyPlayerViewGlobalSize)
{
	RoundedRectangleElement& tableBackground = static_cast<RoundedRectangleElement&>(addChild(std::make_unique<RoundedRectangleElement>(m_size)));
	tableBackground.shape().setFillColor({ 25, 25, 40, 255 });

	const sf::Vector2f& tableBackgroundSize = tableBackground.getSize();

	RoundedRectangleElement& table = static_cast<RoundedRectangleElement&>(addChild(std::make_unique<RoundedRectangleElement>(sf::Vector2f(tableBackgroundSize.x * 0.5f, tableBackgroundSize.y * 0.6f))));
	table.setOrigin(table.getGeometricCenter());
	table.setPosition(tableBackground.getGeometricCenter());
	table.shape().setFillColor(sf::Color::Green);


	//VisualComponent buttonsComponent;
	//buttonsComponent.colors.normal = { 200, 200, 200, 255 };
	//buttonsComponent.colors.hovered = { 200, 200, 255, 255 };
	//buttonsComponent.colors.pressed = sf::Color::Transparent;

	float arrowOffsetX = tableBackgroundSize.x * 0.05f;

	Button& leftArrow = static_cast<Button&>(addChild(std::make_unique<Button>()));
	Button& rightArrow = static_cast<Button&>(addChild(std::make_unique<Button>()));

	leftArrow.setSize({ tableBackgroundSize.y * 0.1f, tableBackgroundSize.y * 0.1f });
	rightArrow.setSize(leftArrow.getSize());

	leftArrow.setOrigin(leftArrow.getGeometricCenter());
	rightArrow.setOrigin(rightArrow.getGeometricCenter());

	leftArrow.setPosition({ arrowOffsetX,tableBackgroundSize.y * 0.5f });
	rightArrow.setPosition({ tableBackgroundSize.x - arrowOffsetX, tableBackgroundSize.y * 0.5f });

	SpriteElement& leftArrowSprite = static_cast<SpriteElement&>(leftArrow.addChild(std::make_unique<SpriteElement>(gameContext.textureManager.getTexture("assets/textures/ui/arrow_decorative_w.png"))));
	SpriteElement& rightArrowSprite = static_cast<SpriteElement&>(rightArrow.addChild(std::make_unique<SpriteElement>(gameContext.textureManager.getTexture("assets/textures/ui/arrow_decorative_w.png"))));
	rightArrow.setRotation(sf::degrees(180.0f));

	leftArrowSprite.fitToSize(leftArrow.getSize());
	rightArrowSprite.fitToSize(rightArrow.getSize());

	//to do interactives

	//leftArrowSprite.sprite().setColor(buttonsComponent.colors.normal);
	//rightArrowSprite.sprite().setColor(buttonsComponent.colors.normal);

	//leftArrow.setOnHover([buttonsComponent, leftArrowSpritePtr = &leftArrowSprite](UIInteractive& element)
	//	{
	//	});

	//rightArrow.setOnHover([buttonsComponent, rightArrowSpritePtr = &rightArrowSprite](UIInteractive& element)
	//	{
	//	});

	//leftArrow.setOnHoverEnd([buttonsComponent, leftArrowSpritePtr = &leftArrowSprite](UIInteractive& element)
	//	{
	//	});

	//rightArrow.setOnHoverEnd([buttonsComponent, rightArrowSpritePtr = &rightArrowSprite](UIInteractive& element)
	//	{
	//	});

	leftArrow.setOnClick([&gameContext, &clientLobbyState]()
		{
			uint32_t currentPlayersPerGame = clientLobbyState.getPlayersPerGame();
			currentPlayersPerGame = currentPlayersPerGame == Constants::Lobby::MinPlayersPerGame ? Constants::Lobby::MaxPlayersPerGame : --currentPlayersPerGame;

			gameContext.ES.publish(PlayersPerGameChangeRequestEvent(currentPlayersPerGame));
		});

	rightArrow.setOnClick([&gameContext, &clientLobbyState]()
		{
			uint32_t currentPlayersPerGame = clientLobbyState.getPlayersPerGame();
			currentPlayersPerGame = currentPlayersPerGame == Constants::Lobby::MaxPlayersPerGame ? Constants::Lobby::MinPlayersPerGame : ++currentPlayersPerGame;

			gameContext.ES.publish(PlayersPerGameChangeRequestEvent(currentPlayersPerGame));
		});


	if (clientContext.localRole == ClientRole::Regular)
	{
		leftArrow.setVisible(false);
		rightArrow.setVisible(false);
	}


	m_seatsPanel = &static_cast<LobbySeatsPanel&>(addChild(std::make_unique<LobbySeatsPanel>(m_size)));
	m_seatsPanel->init(table, lobbyPlayerViewGlobalSize, clientLobbyState);
}

LobbySeatsPanel* LobbyTablePanel::getLobbySeatsPanel()
{
	return m_seatsPanel;
}