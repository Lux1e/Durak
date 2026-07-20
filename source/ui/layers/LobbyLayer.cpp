#include "LobbyLayer.h"
#include "../../core/EventSystem.h"
#include "../../core/EventInitiator.h"
#include "../../core/Constants.h"
#include "../../core/ClientContext.h"
#include "../core/Panel.h"
#include "../../core/animation/MoveAnimation.h"
#include "../../core/animation/RotateAnimation.h"
#include "../../resources/TextureManager.h"
#include "../../utils/MathUtils.h"
#include "../../utils/UIUtils.h"
#include "../../states/client/ClientLobbyState.h"
#include "../panels/LobbyQueuePanel.h"
#include "../panels/LobbySeatsPanel.h"
#include "../panels/LobbyTopLeftPanel.h"
#include "../elements/RectangleElement.h"
#include "../elements/RoundedRectangleElement.h"
#include "../elements/SpriteElement.h"
#include "../widgets/Button.h"
#include "../widgets/Dropdown.h"
#include "../ButtonBuilder.h"
#include "../LobbyPlayerView.h"
#include "../../game/events/requests/SeatPositionsChangeRequestEvent.h"
#include "../../game/events/requests/SeatPositionsSwapRequestEvent.h"
#include "../../game/events/domain/SeatPositionsChangedEvent.h"
#include "../../game/events/domain/SeatPositionsSwappedEvent.h"
#include "../../game/events/requests/ExitRequestEvent.h"



void LobbyLayer::init()
{
	subscribeAll();

	sf::Vector2f backgroundOffset = MathUtils::round({ 0.0f, m_size.y * 0.10f });

	m_screenOffset = MathUtils::round({ m_size.x * 0.025f, m_size.x * 0.025f }); // both offsets have the same value, in Y value is not a mistake
	m_panelsOffset = MathUtils::round({ m_size.x * 0.03f,  m_size.x * 0.03f }); // same logic here

	sf::Vector2f maxSize = { (m_size.x - backgroundOffset.x * 2.0f) - m_screenOffset.x * 2.0f, (m_size.y - backgroundOffset.y * 2.0f) - m_screenOffset.y * 2.0f };
	sf::Vector2f minPosition = { backgroundOffset.x + m_screenOffset.x, backgroundOffset.y + m_screenOffset.y };
	sf::Vector2f maxPosition = { m_size.x - backgroundOffset.x - m_screenOffset.x, m_size.y - backgroundOffset.y - m_screenOffset.y };


	m_backgroundPanel = &addPanel(std::make_unique<Panel>());
	m_topPanel = &addPanel(std::make_unique<Panel>());
	m_queuePanel = &static_cast<LobbyQueuePanel&>(addPanel(std::make_unique<LobbyQueuePanel>()));
	m_arrowRotatePanel = &addPanel(std::make_unique<Panel>());
	m_tablePanel = &addPanel(std::make_unique<Panel>());
	m_chatPanel = &addPanel(std::make_unique<Panel>());
	m_bottomButtonsPanel = &addPanel(std::make_unique<Panel>());
	m_settingsPanel = &addPanel(std::make_unique<Panel>());
	m_popupPanel = &addPanel(std::make_unique<Panel>());
	m_playersPanel = &addPanel(std::make_unique<Panel>());


	m_backgroundPanel->setSize(m_size);

	m_topPanel->setPosition({ 0.0f, backgroundOffset.y });
	m_topPanel->setSize(MathUtils::round({ m_size.x, m_size.y * 0.05f }));

	m_chatPanel->setSize(MathUtils::round({ maxSize.x * 0.80f, m_size.y * 0.15f }));
	m_chatPanel->setPosition({ minPosition.x, maxPosition.y });
	m_chatPanel->setOrigin({ 0.0f, m_chatPanel->getSize().y });

	m_queuePanel->setPosition({ minPosition.x, m_topPanel->getBottomLeftPosition().y + m_panelsOffset.y });
	m_queuePanel->setSize(MathUtils::round({ maxSize.x * 0.10f, m_chatPanel->getTopLeftPosition().y - m_queuePanel->getTopLeftPosition().y - m_panelsOffset.y }));

	m_arrowRotatePanel->setPosition(MathUtils::round({ m_queuePanel->getTopRightPosition().x, m_queuePanel->getTopRightPosition().y + m_queuePanel->getSize().y * 0.5f }));
	m_arrowRotatePanel->setSize(MathUtils::round({ m_queuePanel->getSize().x * 0.8f, m_queuePanel->getSize().x * 0.8f }));
	m_arrowRotatePanel->setOrigin({ 0.0f, m_arrowRotatePanel->getSize().y * 0.5f });

	m_bottomButtonsPanel->setPosition({ maxPosition.x, maxPosition.y });
	m_bottomButtonsPanel->setSize({ maxPosition.x - m_chatPanel->getTopRightPosition().x - m_panelsOffset.x, m_chatPanel->getSize().y });
	m_bottomButtonsPanel->setOrigin({ m_bottomButtonsPanel->getSize().x, m_bottomButtonsPanel->getSize().y });

	m_tablePanel->setPosition({ m_arrowRotatePanel->getTopRightPosition().x, m_queuePanel->getTopRightPosition().y });
	m_tablePanel->setSize({ m_chatPanel->getTopRightPosition().x - m_tablePanel->getPosition().x, m_queuePanel->getSize().y });

	m_settingsPanel->setPosition({ m_tablePanel->getTopRightPosition().x + m_panelsOffset.x, m_tablePanel->getTopRightPosition().y });
	m_settingsPanel->setSize({ maxPosition.x - m_settingsPanel->getTopLeftPosition().x, m_queuePanel->getSize().y });

	m_popupPanel->setSize(m_size);
	m_playersPanel->setSize(m_size);


	initBackgroundPanel(backgroundOffset);
	initTopPanel();
	m_queuePanel->init();
	m_lobbyPlayerViewGlobalSize = m_queuePanel->getPlayerViewGlobalSize();
	initArrowRotatePanel();
	initTablePanel();

	initChatPanel();
	initBottomButtonsPanel();
	initSettingsPanel();
	initPopupPanel();
	initPlayersPanel();

	initTestPanel();
}


void LobbyLayer::initBackgroundPanel(sf::Vector2f backgroundOffset)
{
	RectangleElement& firstBackground = static_cast<RectangleElement&>(m_backgroundPanel->addChild(std::make_unique<RectangleElement>(m_backgroundPanel->getSize())));
	RectangleElement& secondBackground = static_cast<RectangleElement&>(m_backgroundPanel->addChild(std::make_unique<RectangleElement>()));

	firstBackground.shape().setFillColor({ 20, 20, 20, 255 });

	secondBackground.setSize({ m_backgroundPanel->getSize().x, m_backgroundPanel->getSize().y - backgroundOffset.y * 2.0f });
	secondBackground.setOrigin(secondBackground.getGeometricCenter());
	secondBackground.setPosition(m_backgroundPanel->getGeometricCenter());
	secondBackground.shape().setFillColor({ 30, 30, 40, 255 });
}

void LobbyLayer::initTopPanel()
{
	RoundedRectangleElement& background = static_cast<RoundedRectangleElement&>(m_topPanel->addChild(std::make_unique<RoundedRectangleElement>(m_topPanel->getSize())));
	background.shape().setFillColor({ 60, 60, 80, 255 });

	m_topLeftPanel = &static_cast<LobbyTopLeftPanel&>(m_topPanel->addChild(std::make_unique<LobbyTopLeftPanel>()));
	m_topLeftPanel->setSize({ m_queuePanel->getSize().x, m_topPanel->getSize().y * 0.8f });
	m_topLeftPanel->setPosition({ m_screenOffset.x, m_topPanel->getSize().y * 0.1f });

	m_topLeftPanel->init(m_clientLobbyState, m_gameContext, m_clientContext);
}

void LobbyLayer::initArrowRotatePanel()
{
	SpriteElement& arrowRotate = static_cast<SpriteElement&>(m_arrowRotatePanel->addChild(std::make_unique<SpriteElement>(m_gameContext.textureManager.getTexture("assets/textures/ui/arrow_rotate.png"))));
	arrowRotate.setPosition(m_arrowRotatePanel->getGeometricCenter());
	arrowRotate.setSize({ m_arrowRotatePanel->getSize().x * 0.8f, m_arrowRotatePanel->getSize().y * 0.8f });
	arrowRotate.setOrigin(arrowRotate.getGeometricCenter());
	arrowRotate.sprite().setColor({ 255, 255, 255, 10 });
}

void LobbyLayer::initTablePanel()
{
	RoundedRectangleElement& tableBackground = static_cast<RoundedRectangleElement&>(m_tablePanel->addChild(std::make_unique<RoundedRectangleElement>(m_tablePanel->getSize())));
	tableBackground.shape().setFillColor({ 25, 25, 40, 255 });

	const sf::Vector2f& tableBackgroundSize = tableBackground.getSize();

	RoundedRectangleElement& table = static_cast<RoundedRectangleElement&>(m_tablePanel->addChild(std::make_unique<RoundedRectangleElement>(sf::Vector2f(tableBackgroundSize.x * 0.5f, tableBackgroundSize.y * 0.6f))));
	table.setOrigin(table.getGeometricCenter());
	table.setPosition(tableBackground.getGeometricCenter());
	table.shape().setFillColor(sf::Color::Green);

	m_seatsPanel = &static_cast<LobbySeatsPanel&>(m_tablePanel->addChild(std::make_unique<LobbySeatsPanel>(m_tablePanel->getSize())));
	m_seatsPanel->init(table, m_lobbyPlayerViewGlobalSize, m_clientLobbyState.getPlayersPerGame());
}

void LobbyLayer::initChatPanel() //to do
{
	RoundedRectangleElement& chatBox = static_cast<RoundedRectangleElement&>(m_chatPanel->addChild(std::make_unique<RoundedRectangleElement>(m_chatPanel->getSize())));
	chatBox.shape().setFillColor(sf::Color::Cyan);
}

void LobbyLayer::initBottomButtonsPanel()
{
	const sf::Vector2f& panelSize = m_bottomButtonsPanel->getSize();

	RoundedRectangleElement& background = static_cast<RoundedRectangleElement&>(m_bottomButtonsPanel->addChild(std::make_unique<RoundedRectangleElement>(m_bottomButtonsPanel->getSize())));
	background.shape().setFillColor(sf::Color::Yellow);

	ButtonBuilder builder("READY", { panelSize.x * 0.90f, panelSize.y * 0.40f });
	ButtonStyle readyButtonStyle = ButtonStyleFactory::makeStandardStyle({ 40, 200, 40, 255 }, sf::Color::Black);
	readyButtonStyle.text.outline.width = 0.0f;
	readyButtonStyle.text.sizeFactor = { 0.8f, 0.65f };
	readyButtonStyle.background.outline.width = -2.5f;
	builder.setStyle(readyButtonStyle);
	builder.setFont(Fonts::ArialBI);
	m_readyButton = &static_cast<Button&>(m_bottomButtonsPanel->addChild(builder.build()));

	builder.setText("START");
	ButtonStyle startButtonStyle = ButtonStyleFactory::makeStandardStyle({ 40, 40, 200, 255 }, sf::Color::White);
	startButtonStyle.text.outline.width = 0.0f;
	startButtonStyle.text.sizeFactor = { 0.8f, 0.65f };
	startButtonStyle.background.outline.width = -2.5f;
	builder.setStyle(startButtonStyle);
	m_startButton = &static_cast<Button&>(m_bottomButtonsPanel->addChild(builder.build()));

	m_readyButton->setOrigin({ m_readyButton->getSize().x * 0.5f, 0.0f });
	m_startButton->setOrigin({ m_startButton->getSize().x * 0.5f, m_startButton->getSize().y });

	float totalOffsetY = panelSize.y - (m_readyButton->getSize().y + m_startButton->getSize().y);

	m_readyButton->setPosition({ panelSize.x * 0.5f, totalOffsetY / 3.0f });
	m_startButton->setPosition({ panelSize.x * 0.5f, panelSize.y - totalOffsetY / 3.0f });
}

void LobbyLayer::initSettingsPanel()
{
	m_settingsPanel->setTransparentToInput(true);

	RectangleElement& background = static_cast<RectangleElement&>(m_settingsPanel->addChild(std::make_unique<RectangleElement>(m_settingsPanel->getSize())));
	background.shape().setFillColor({ 20, 20, 35, 255 });

	sf::Vector2f localOffset = { background.getSize().x * 0.05f, background.getSize().y * 0.05f };

	TextElement& settings = static_cast<TextElement&>(m_settingsPanel->addChild(std::make_unique<TextElement>(Fonts::ArialBD, "Settings")));
	settings.setSize({ background.getSize().x * 0.6f, background.getSize().y * 0.06f });
	settings.setOrigin({ settings.getSize().x * 0.5f, 0.0f });
	settings.setPosition({ background.getSize().x * 0.5f, localOffset.y });

	Panel& localSettingsPanel = static_cast<Panel&>(m_settingsPanel->addChild(std::make_unique<Panel>()));
	localSettingsPanel.setSize({ m_settingsPanel->getSize().x - localOffset.x * 2, m_settingsPanel->getSize().y - settings.getBottomLeftPosition().y - localOffset.y * 2 });
	localSettingsPanel.setOrigin({ localSettingsPanel.getSize().x * 0.5f, 0.0f });
}

void LobbyLayer::initPopupPanel()
{
	m_popupPanel->setTransparentToInput(true);
}

void LobbyLayer::initPlayersPanel()
{
	m_playersPanel->setTransparentToInput(true);

	for (const auto& [id, playerData] : m_clientLobbyState.getAllPlayersById())
		addPlayer(id, playerData.getNickname(), playerData.getRole(), m_clientLobbyState.getPlayerSeatIndex(id).value());
}


void LobbyLayer::initTestPanel() // TEST PANEL
{
}


void LobbyLayer::subscribeAll()
{
	m_gameContext.ES.subscribe<SeatPositionsChangedEvent>(this, &LobbyLayer::onSeatPositionsChangedEvent);
	m_gameContext.ES.subscribe<SeatPositionsSwappedEvent>(this, &LobbyLayer::onSeatPositionsSwappedEvent);
}


void LobbyLayer::rebuildLayer() // to do
{
	assert(false);
}

void LobbyLayer::resyncLayer() // to do
{
	assert(false);
}


void LobbyLayer::addPlayer(uint32_t id, std::string nickname, ClientRole role, int seatIndex)
{
	assert(m_playersById.find(id) == m_playersById.end());

	LobbyPlayerView& player = static_cast<LobbyPlayerView&>(m_playersPanel->addChild(std::make_unique<LobbyPlayerView>(id, std::move(nickname), role)));
	m_playersById[id] = &player;

	auto inverse = m_playersPanel->getGlobalTransform().getInverse();

	player.setSize({ m_lobbyPlayerViewGlobalSize.x / m_playersPanel->getGlobalScale().x, m_lobbyPlayerViewGlobalSize.y / m_playersPanel->getGlobalScale().y });
	player.setOrigin(player.getGeometricCenter());

	if (seatIndex == Constants::Lobby::QueueSeat)
	{
		if (auto seatIndexOpt = m_clientLobbyState.getPlayerIndexInQueueById(id); seatIndexOpt)
			player.setPosition(MathUtils::round(inverse.transformPoint(m_queuePanel->getPlayerGlobalSeatPositionByQueueIndex(seatIndexOpt.value()))));
		else
			assert(false);
	}

	else
		player.setPosition(MathUtils::round(inverse.transformPoint(m_seatsPanel->getGlobalSeatPositionBySeatIndex(seatIndex))));
}

void LobbyLayer::removePlayer(uint32_t id)
{
	if (m_playersById.find(id) == m_playersById.end())
	{
		resyncLayer();
		return;
	}

	LobbyPlayerView* player = m_playersById.find(id)->second;

	removePlayer(*player);
}

void LobbyLayer::removePlayer(LobbyPlayerView& player)
{
	const uint32_t id = player.getId();
	auto seatIndexOpt = m_clientLobbyState.getPlayerSeatIndex(id);
	auto& children = m_playersPanel->getAllChildren();

	auto itMap = m_playersById.find(id);
	auto itVec = std::find_if(children.begin(), children.end(), [&player](const std::unique_ptr<UIElement>& child)
		{
			return &player == child.get();
		});

	if (!seatIndexOpt || itMap == m_playersById.end() || itVec == children.end())
	{
		resyncLayer();
		return;
	}

	assert(&player == itMap->second);
	assert(&player == itVec->get());

	m_playersById.erase(itMap);
	children.erase(itVec);

	if (seatIndexOpt.value() == Constants::Lobby::QueueSeat)
		updatePlayersSeatsInQueue();
}


bool LobbyLayer::handleEvents(const sf::Event& event)
{
	if (const auto& e = event.getIf<sf::Event::MouseButtonPressed>())
	{
		if (e->button == sf::Mouse::Button::Left)
		{
			if (m_underMouseElement)
			{
				if (m_underMouseElement->isDraggable())
				{
					m_dragController.startDragging(m_underMouseElement);

					auto& children = m_underMouseElement->getParent()->getAllChildren();
					for (size_t i = 0; children.size(); ++i)
					{
						if (children[i].get() == m_underMouseElement)
						{
							std::swap(children[i], children.back());
							break;
						}
					}
				}

				else
					return onMousePressedEvent();
			}
		}
	}

	if (const auto& e = event.getIf<sf::Event::MouseButtonReleased>())
	{
		if (e->button == sf::Mouse::Button::Left)
		{
			if (m_underMouseElement)
			{
				if (m_underMouseElement->isDraggable() && m_dragController.isDragging())
				{
					if (auto* player = dynamic_cast<LobbyPlayerView*>(m_underMouseElement))
						return onLobbyPlayerViewDrop(*player);
				}

				else
					return onMouseReleasedEvent();
			}
		}
	}

	return false;
}

void LobbyLayer::update(float dt)
{
	m_dragController.update(m_gameContext.input);

	for (const auto& panel : m_panels)
		panel.get()->update(dt);
}


bool LobbyLayer::onLobbyPlayerViewDrop(LobbyPlayerView& player)
{
	m_dragController.endDragging();

	uint32_t playerId = player.getId();
	bool inQueue = false;
	int currentPlayerSeatIndex = m_clientLobbyState.getPlayerSeatIndex(playerId).value();
	std::optional<sf::Vector2f> currentPlayerGlobalSeatPosition = std::nullopt;

	if (currentPlayerSeatIndex == Constants::Lobby::QueueSeat)
	{
		inQueue = true;
		currentPlayerSeatIndex = static_cast<int>(m_clientLobbyState.getPlayerIndexInQueueById(playerId).value());
		currentPlayerGlobalSeatPosition = m_queuePanel->getPlayerGlobalSeatPositionByQueueIndex(currentPlayerSeatIndex);
	}

	else
		currentPlayerGlobalSeatPosition = m_seatsPanel->getGlobalSeatPositionBySeatIndex(currentPlayerSeatIndex);

	assert(currentPlayerGlobalSeatPosition);
	sf::Vector2f currentPlayerLocalSeatPosition = player.toParentSpace(*currentPlayerGlobalSeatPosition);

	sf::Vector2f endPosition = currentPlayerLocalSeatPosition;

	if (auto* seat = m_seatsPanel->hitTest(m_gameContext.input.mouseWorldPosition))
	{
		int seatIndex = static_cast<int>(m_seatsPanel->getSeatIndexBySeatElement(*seat));

		if (inQueue || seatIndex != currentPlayerSeatIndex)
		{
			if (auto otherPlayerIdOpt = m_clientLobbyState.getPlayerIdBySeatIndex(seatIndex))
			{
				if (canSwapPositions())
					m_gameContext.ES.publish<SeatPositionsSwapRequestEvent>({ playerId, otherPlayerIdOpt.value() }, m_clientContext.localId);
			}

			else
			{
				if (canChangePosition())
					m_gameContext.ES.publish<SeatPositionsChangeRequestEvent>({ playerId, seatIndex }, m_clientContext.localId);
			}
		}
	}

	else if (m_queuePanel->hitTest(m_gameContext.input.mouseWorldPosition))
	{
		if (!inQueue && canChangePosition())
			m_gameContext.ES.publish<SeatPositionsChangeRequestEvent>({ playerId, Constants::Lobby::QueueSeat }, m_clientContext.localId);
	}

	applyMoveAnimationToPlayer<MoveAnimation>(player, [playerPtr = &player](sf::Vector2f pos) { playerPtr->setPosition(pos); },
		EaseType::InOutCubic, player.getPosition(), endPosition, Constants::Animations::StandardMoveAnimationTime);

	return true;
}


bool LobbyLayer::canChangePosition()
{
	if (m_clientContext.localRole == ClientRole::Host)
		return true;

	else
		return m_clientLobbyState.canMoveToAnotherSeat();
}

bool LobbyLayer::canSwapPositions()
{
	if (m_clientContext.localRole != ClientRole::Host)
		return false;

	return true;
}


void LobbyLayer::updatePlayersSeatsInQueue()
{
	const std::vector<uint32_t>& ids = m_clientLobbyState.getAllPlayersIdsInQueue();

	for (const auto& id : ids)
	{
		auto queueIndexOpt = m_clientLobbyState.getPlayerIndexInQueueById(id);
		assert(queueIndexOpt);

		sf::Vector2f playerGlobalSeatPosition = m_queuePanel->getPlayerGlobalSeatPositionByQueueIndex(queueIndexOpt.value());

		if (auto it = m_playersById.find(id); it != m_playersById.end())
		{
			LobbyPlayerView& player = *it->second;

			if (player.getGlobalPosition() == playerGlobalSeatPosition || m_dragController.getDraggingElement() == &player)
				continue;

			applyMoveAnimationToPlayer<MoveAnimation>(player, [playerPtr = &player](sf::Vector2f pos) { playerPtr->setPosition(pos); },
				EaseType::InOutCubic, player.getPosition(), player.toParentSpace(playerGlobalSeatPosition), Constants::Animations::StandardMoveAnimationTime);
		}

		else
		{
			resyncLayer();
			return;
		}
	}
}


void LobbyLayer::onSeatPositionsChangedEvent(const SeatPositionsChangedEvent& event, const EventInitiator& initiator)
{
	auto it = m_playersById.find(event.playerId);
	if (it == m_playersById.end())
	{
		resyncLayer();
		return;
	}

	LobbyPlayerView& player = *it->second;
	sf::Vector2f globalSeatPosition;

	if (event.newSeatIndex == Constants::Lobby::QueueSeat)
	{
		auto queueIndexOpt = m_clientLobbyState.getPlayerIndexInQueueById(event.playerId);
		if (!queueIndexOpt)
		{
			resyncLayer();
			return;
		}

		globalSeatPosition = m_queuePanel->getPlayerGlobalSeatPositionByQueueIndex(queueIndexOpt.value());
	}

	else
		globalSeatPosition = m_seatsPanel->getGlobalSeatPositionBySeatIndex(event.newSeatIndex);

	sf::Vector2f localSeatPosition = player.toParentSpace(globalSeatPosition);
	sf::Vector2f currentLocalPosition = player.getPosition();

	if (localSeatPosition == currentLocalPosition)
		return;

	applyMoveAnimationToPlayer<MoveAnimation>(player, [playerPtr = &player](sf::Vector2f pos) { playerPtr->setPosition(pos); },
		EaseType::InOutCubic, currentLocalPosition, localSeatPosition, Constants::Animations::StandardMoveAnimationTime);

	updatePlayersSeatsInQueue();
}

void LobbyLayer::onSeatPositionsSwappedEvent(const SeatPositionsSwappedEvent& event, const EventInitiator& initiator)
{
	auto firstPlayerIt = m_playersById.find(event.firstPlayerId);
	auto secondPlayerIt = m_playersById.find(event.secondPlayerId);

	auto firstPlayerSeatIndex = m_clientLobbyState.getPlayerSeatIndex(event.firstPlayerId);
	auto secondPlayerSeatIndex = m_clientLobbyState.getPlayerSeatIndex(event.secondPlayerId);

	if (firstPlayerIt == m_playersById.end() || secondPlayerIt == m_playersById.end() || !firstPlayerSeatIndex || !secondPlayerSeatIndex || (firstPlayerSeatIndex == secondPlayerSeatIndex))
	{
		resyncLayer();
		return;
	}


	LobbyPlayerView& firstPlayer = *firstPlayerIt->second;
	LobbyPlayerView& secondPlayer = *secondPlayerIt->second;

	sf::Vector2f firstPlayerGlobalSeatPosition;
	sf::Vector2f secondPlayerGlobalSeatPosition;


	if (firstPlayerSeatIndex.value() == Constants::Lobby::QueueSeat)
	{
		auto firstPlayerQueueIndexOpt = m_clientLobbyState.getPlayerIndexInQueueById(firstPlayer.getId());
		if (!firstPlayerQueueIndexOpt)
		{
			resyncLayer();
			return;
		}

		firstPlayerGlobalSeatPosition = m_queuePanel->getPlayerGlobalSeatPositionByQueueIndex(firstPlayerQueueIndexOpt.value());
	}

	else
		firstPlayerGlobalSeatPosition = m_seatsPanel->getGlobalSeatPositionBySeatIndex(firstPlayerSeatIndex.value());


	if (secondPlayerSeatIndex.value() == Constants::Lobby::QueueSeat)
	{
		auto secondPlayerQueueIndexOpt = m_clientLobbyState.getPlayerIndexInQueueById(secondPlayer.getId());
		if (!secondPlayerQueueIndexOpt)
		{
			resyncLayer();
			return;
		}

		secondPlayerGlobalSeatPosition = m_queuePanel->getPlayerGlobalSeatPositionByQueueIndex(secondPlayerQueueIndexOpt.value());
	}

	else
		secondPlayerGlobalSeatPosition = m_seatsPanel->getGlobalSeatPositionBySeatIndex(secondPlayerSeatIndex.value());


	sf::Vector2f firstPlayerCurrentLocalPosition = firstPlayer.getPosition();
	sf::Vector2f secondPlayerCurrentLocalPosition = secondPlayer.getPosition();

	sf::Vector2f firstPlayerLocalSeatPosition = firstPlayer.toParentSpace(firstPlayerGlobalSeatPosition);
	sf::Vector2f secondPlayerLocalSeatPosition = secondPlayer.toParentSpace(secondPlayerGlobalSeatPosition);


	if (firstPlayerCurrentLocalPosition != firstPlayerLocalSeatPosition)
		applyMoveAnimationToPlayer<MoveAnimation>(firstPlayer, [playerPtr = &firstPlayer](sf::Vector2f pos) { playerPtr->setPosition(pos); },
			EaseType::InOutCubic, firstPlayerCurrentLocalPosition, firstPlayerLocalSeatPosition, Constants::Animations::StandardMoveAnimationTime);

	if (secondPlayerCurrentLocalPosition != secondPlayerLocalSeatPosition)
		applyMoveAnimationToPlayer<MoveAnimation>(secondPlayer, [playerPtr = &secondPlayer](sf::Vector2f pos) { playerPtr->setPosition(pos); },
			EaseType::InOutCubic, secondPlayerCurrentLocalPosition, secondPlayerLocalSeatPosition, Constants::Animations::StandardMoveAnimationTime);
}