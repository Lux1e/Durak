#include "TestLobbyScene.h"
#include "../../states/client/ClientLobbyState.h"
#include "../../core/input/InputState.h"
#include "../../ui/animation/EaseInOutCubicMoveAnimation.h"
#include "../../core/EventSystem.h"
#include "../../core/EventInitiator.h"
#include "../../game/events/domain/SnapshotAppliedEvent.h"
#include "../../game/events/domain/PlayerConnectedEvent.h"
#include "../../game/events/domain/SeatPositionsChangedEvent.h"
#include "../../game/events/domain/SeatPositionsSwappedEvent.h"
#include "../../game/events/requests/SeatPositionsChangeRequestEvent.h"
#include "../../game/events/requests/SeatPositionsSwapRequestEvent.h"



void TestLobbyScene::init(const sf::Vector2f& windowSize)
{
	subscribeAll();

	spacingFromRoundedBackgroundRatio = { 0.90f, 0.90f };
	spacingUnderTopBoxRatio = 0.33f;
	spacingBetweenQueueBoxAndTableBoxRatio = 0.50f;


	background.setSize(windowSize);
	background.setFillColor(sf::Color::Black);

	roundedBackground.setCornerRatio(0.05f);
	roundedBackground.setSize({ background.getSize().x * 0.97f, background.getSize().y * 0.97f });
	roundedBackground.setFillColor({ 20, 20, 25 });
	roundedBackground.setOrigin(roundedBackground.getGeometricCenter());
	roundedBackground.setPosition({ windowSize.x / 2.0f, windowSize.y / 2.0f });

	auto roundedBackgroundSize = roundedBackground.getSize();

	sf::RectangleShape invisibleBox;
	invisibleBox.setSize({ roundedBackgroundSize.x * spacingFromRoundedBackgroundRatio.x, roundedBackgroundSize.y * spacingFromRoundedBackgroundRatio.y });
	invisibleBox.setOrigin(invisibleBox.getGeometricCenter());
	invisibleBox.setPosition(roundedBackground.getPosition());
	invisibleBox.setFillColor(sf::Color::Yellow);

	auto invisibleBoxSize = invisibleBox.getSize();
	auto invisibleBoxPosition = invisibleBox.getGlobalBounds().position;

	topBox.setSize({ invisibleBoxSize.x, invisibleBoxSize.y * 0.15f });
	topBox.setFillColor({ 120, 130, 140 });
	topBox.setOutlineThickness(2.0f);
	topBox.setOutlineColor(sf::Color::Black);
	topBox.setPosition({ invisibleBoxPosition });

	auto topBoxSize = topBox.getSize();

	queueBox.setSize({ invisibleBoxSize.x * 0.15f, invisibleBoxSize.y - topBoxSize.y - topBoxSize.y * spacingUnderTopBoxRatio });
	queueBox.setFillColor({ 60, 65, 75 });
	queueBox.setOutlineThickness(2.0f);
	queueBox.setOutlineColor(sf::Color::Black);
	queueBox.setPosition({ invisibleBoxPosition.x, invisibleBoxPosition.y + topBoxSize.y + topBoxSize.y * spacingUnderTopBoxRatio });

	auto queueBoxSize = queueBox.getSize();
	auto queueBoxPosition = queueBox.getGlobalBounds().position;

	playerViewBoxSize = { queueBoxSize.x, queueBoxSize.y / (static_cast<float>(Constants::Lobby::MaxPlayersInLobby) + 1.5f) };

	playersLabel.setString("PLAYERS:");
	auto& text = playersLabel.getText();
	text.setFillColor(sf::Color::Cyan);
	text.setFont(Fonts::ArialBD);
	text.setCharacterSize(playerViewBoxSize.y / 2);
	playersLabel.setCenterOrigin();
	playersLabel.setPosition({
		queueBoxPosition.x + queueBoxSize.x / 2,
		queueBoxPosition.y + playerViewBoxSize.y * 1.5f / 2
		});

	tableBox.setSize({ invisibleBoxSize.x - queueBoxSize.x - queueBoxSize.x * spacingBetweenQueueBoxAndTableBoxRatio, queueBoxSize.y });
	tableBox.setFillColor({ 60, 65, 75 });
	tableBox.setCornerRatio(0.05f);
	tableBox.setOutlineThickness(2.0f);
	tableBox.setOutlineColor(sf::Color::Black);
	tableBox.setPosition({ invisibleBoxPosition.x + queueBoxSize.x + queueBoxSize.x * spacingBetweenQueueBoxAndTableBoxRatio, queueBoxPosition.y });

	table.setFillColor({ 0, 150, 0 });
	table.setOutlineThickness(2.0f);
	table.setOutlineColor({ 75, 75, 75 });
	table.setSize({ tableBox.getSize().x * 0.5f, tableBox.getSize().y * 0.5f });
	table.setOrigin(table.getGeometricCenter());
	table.setPosition({ tableBox.getGlobalBounds().position.x + tableBox.getGlobalBounds().size.x / 2,
		tableBox.getGlobalBounds().position.y + tableBox.getGlobalBounds().size.y / 2 });


	initQueueLayout();
	initTableLayout();

	tableSeatBoxes.reserve(Constants::Lobby::MaxPlayersPerGame);
	initTableSeatBoxes();

	players.reserve(Constants::Lobby::MaxPlayersInLobby);
	playerById.reserve(Constants::Lobby::MaxPlayersInLobby);

	draggedPlayer = nullptr;
}

void TestLobbyScene::initQueueLayout()
{
	sf::Vector2f boxForQueuePlayersPosition = queueBox.getPosition();
	sf::Vector2f boxForQueuePlayersSize = queueBox.getSize();

	sf::Vector2f firstQueuePosition = {
		boxForQueuePlayersPosition.x + boxForQueuePlayersSize.x / 2,
		boxForQueuePlayersPosition.y + playerViewBoxSize.y / 2 + playerViewBoxSize.y * 1.5f };

	float positionOffset = playerViewBoxSize.y;

	for (size_t i = 0; i < Constants::Lobby::MaxPlayersInLobby; ++i)
	{
		queueLayout.setPosition(i, { firstQueuePosition.x, firstQueuePosition.y + (positionOffset * i) });
	}
}

void TestLobbyScene::initTableLayout()
{
	const uint32_t playersPerGame = m_lobbyStatePtr ? m_lobbyStatePtr->getPlayersPerGame() : Constants::Lobby::MinPlayersPerGame;

	if (playersPerGame < Constants::Lobby::MinPlayersPerGame || playersPerGame > Constants::Lobby::MaxPlayersPerGame)
		return;

	tableLayout.clearAndResize(playersPerGame);

	sf::Vector2f tableCenterPosition = table.getPosition();
	float halfSizeX = table.getSize().x / 2;
	float halfSizeY = table.getSize().y / 2;

	sf::Vector2f bottomPosition = { tableCenterPosition.x, tableCenterPosition.y + halfSizeY };
	sf::Vector2f topPosition = { tableCenterPosition.x, tableCenterPosition.y - halfSizeY };
	sf::Vector2f leftPosition = { tableCenterPosition.x - halfSizeX, tableCenterPosition.y };
	sf::Vector2f rightPosition = { tableCenterPosition.x + halfSizeX, tableCenterPosition.y };

	switch (playersPerGame)
	{
		case 2:
			tableLayout.setPosition(0, bottomPosition);
			tableLayout.setPosition(1, topPosition);
			break;

		case 3:
			tableLayout.setPosition(0, bottomPosition);
			tableLayout.setPosition(1, leftPosition);
			tableLayout.setPosition(2, rightPosition);
			break;

		case 4:
			tableLayout.setPosition(0, bottomPosition);
			tableLayout.setPosition(1, leftPosition);
			tableLayout.setPosition(2, topPosition);
			tableLayout.setPosition(3, rightPosition);
			break;
	}
}


void TestLobbyScene::initTableSeatBoxes()
{
	tableSeatBoxes.clear();

	const size_t size = m_lobbyStatePtr ? m_lobbyStatePtr->getPlayersPerGame() : Constants::Lobby::MinPlayersPerGame;

	for (size_t i = 0; i < size; ++i)
	{
		auto& box = tableSeatBoxes.emplace_back(playerViewBoxSize);
		box.setFillColor(sf::Color::Black);
		box.setSize(playerViewBoxSize);
		box.setOrigin(box.getGeometricCenter());
		box.setPosition(*tableLayout.getPosition(i));
	}
}


void TestLobbyScene::clearAllVisuals()
{
	players.clear();
	playerById.clear();
}

void TestLobbyScene::rebuildFullScene()
{
	clearAllVisuals();

	initTableLayout();
	initTableSeatBoxes();

	for (const auto& [id, ClientPlayerData] : m_lobbyStatePtr->getAllPlayersById())
	{
		players.emplace_back(std::make_unique<LobbyPlayerView>(id, ClientPlayerData.getNickname()));
		playerById.insert_or_assign(id, players.back().get());
		players.back()->setSize(playerViewBoxSize);
	}

	const std::vector<uint32_t>& playersIdsInQueue = m_lobbyStatePtr->getAllPlayersIdsInQueue();
	for (size_t i = 0; i < playersIdsInQueue.size(); ++i)
	{
		playerById.at(playersIdsInQueue[i])->setPosition(queueLayout.getPositionByIndex(i));
	}

	const std::vector<uint32_t> playersIdsInSeatPositions = m_lobbyStatePtr->getAllPlayersIdsInSeatPositions();
	for (size_t i = 0; i < playersIdsInSeatPositions.size(); ++i)
	{
		playerById.at(playersIdsInSeatPositions[i])->setPosition(*tableLayout.getPosition(i));
	}
}


void TestLobbyScene::subscribeAll()
{
	ES.subscribe<SnapshotAppliedEvent, TestLobbyScene>(this, &TestLobbyScene::onSnapshotAppliedEvent);
	ES.subscribe<PlayerConnectedEvent, TestLobbyScene>(this, &TestLobbyScene::onPlayerConnectedEvent);
	ES.subscribe<SeatPositionsChangedEvent, TestLobbyScene>(this, &TestLobbyScene::onSeatPositionsChangedEvent);
	ES.subscribe<SeatPositionsSwappedEvent, TestLobbyScene>(this, &TestLobbyScene::onSeatPositionsSwappedEvent);
}


void TestLobbyScene::update(float dt)
{
	updatePlayers(dt);

	LobbyPlayerView* hovered = nullptr;

	for (auto it = players.rbegin(); it != players.rend(); ++it)
	{
		auto& player = *it->get();
		if (player.containsPoint(input.mouseWorldPosition))
		{
			if (localContext.localRole == ClientRole::Host || player.getId() == localContext.localId)
			{
				if (player.canBeHovered())
					hovered = &player;
			}

			break;
		}
	}

	for (auto& player : players)
	{
		if (player.get() == hovered)
			player->setHovered(true);
		else
			player->setHovered(false);
	}


	int leftMouseButtonInt = static_cast<int>(sf::Mouse::Button::Left);

	if (input.mouseButtons[leftMouseButtonInt].pressed)
	{
		for (int i = players.size() - 1; i >= 0; --i)
		{
			if (players[i]->isHovered())
			{
				auto ptr = std::move(players[i]);
				players.erase(players.begin() + i);
				players.emplace_back(std::move(ptr));

				draggedPlayer = players.back().get();
				draggedPlayer->setPressed(true);

				return;
			}
		}
	}

	if (input.mouseButtons[leftMouseButtonInt].released)
	{
		if (draggedPlayer)
		{
			uint32_t draggedPlayerId = *getPlayerId(*draggedPlayer);
			auto draggedPlayerCopy = draggedPlayer;
			int draggedPlayerSeatPosition = *m_lobbyStatePtr->getPlayerSeatPosition(draggedPlayerId);

			auto positionOpt = hitTableSeatBox(input.mouseWorldPosition);
			if (positionOpt && *positionOpt != draggedPlayerSeatPosition)
			{
				if (auto otherPlayerIdOpt = m_lobbyStatePtr->getPlayerIdBySeatPosition(*positionOpt))
				{
					if (localContext.localRole == ClientRole::Host)
						ES.publish<SeatPositionsSwapRequestEvent>({ draggedPlayerId, *otherPlayerIdOpt }, localContext.localId);
					else
						addPlayerReturnAnimation<EaseInOutCubicMoveAnimation>(draggedPlayerId, Constants::Animations::StandardMoveAnimationTime);
				}

				else
					ES.publish<SeatPositionsChangeRequestEvent>({ draggedPlayerId, *positionOpt }, localContext.localId);
			}

			else if (queueBox.getGlobalBounds().contains(input.mouseWorldPosition) && draggedPlayerSeatPosition != Constants::Lobby::QueueSeat)
				ES.publish<SeatPositionsChangeRequestEvent>({ draggedPlayerId, Constants::Lobby::QueueSeat }, localContext.localId);
			else
				addPlayerReturnAnimation<EaseInOutCubicMoveAnimation>(draggedPlayerId, Constants::Animations::StandardMoveAnimationTime);


			draggedPlayer->setPressed(false);
			draggedPlayer = nullptr;
		}
	}

	if (draggedPlayer)
		draggedPlayer->move(input.mouseDeltaWorld);
}

void TestLobbyScene::handleEvents(const sf::Event& event)
{
}


void TestLobbyScene::render(sf::RenderWindow& window, sf::RenderStates states)
{
	window.draw(background);
	window.draw(roundedBackground);
	window.draw(topBox);
	window.draw(tableBox);
	window.draw(table);
	window.draw(queueBox);
	playersLabel.draw(window, states);

	for (const auto& seat : tableSeatBoxes)
		window.draw(seat);

	drawAllPlayers(window, states);
}


void TestLobbyScene::drawAllPlayers(sf::RenderWindow& window, sf::RenderStates states)
{
	for (auto it = players.begin(); it != players.end(); ++it)
		it->get()->draw(window, states);
}


void TestLobbyScene::updatePlayers(float dt)
{
	for (auto& player : players)
		player->update(dt);
}


std::optional<int> TestLobbyScene::hitTableSeatBox(sf::Vector2f point) const
{
	for (int i = 0; i < tableSeatBoxes.size(); ++i)
	{
		if (tableSeatBoxes[i].getGlobalBounds().contains(point))
			return i;
	}

	return std::nullopt;
}


void TestLobbyScene::addPlayer(uint32_t playerId, const std::string& nickname, ClientRole role, int seatPosition)
{
	auto it = playerById.find(playerId);
	if (it != playerById.end())
		deletePlayer(playerId);

	players.emplace_back(std::make_unique<LobbyPlayerView>(playerId, nickname, playerViewBoxSize));
	LobbyPlayerView& player = *players.back().get();
	player.setSize(playerViewBoxSize);
	playerById[playerId] = &player;

	if (role == ClientRole::Host)
	{
		//to do
	}

	if (playerId == localContext.localId)
	{
		//to do
	}

	if (seatPosition == Constants::Lobby::QueueSeat)
		player.setPosition(queueLayout.getPositionByIndex(*m_lobbyStatePtr->getPlayerIndexInQueueById(playerId)));
	else
		player.setPosition(*tableLayout.getPosition(seatPosition));
}

void TestLobbyScene::deletePlayer(uint32_t id)
{
	auto it = playerById.find(id);
	if (it == playerById.end())
		return;

	LobbyPlayerView* player = it->second;

	if (draggedPlayer == player)
		draggedPlayer == nullptr;

	std::erase_if(players, [player](auto& p) { return p.get() == player; });
	playerById.erase(id);
}


std::optional<uint32_t> TestLobbyScene::getPlayerId(const LobbyPlayerView& player) const
{
	for (const auto& [id, p] : playerById)
	{
		if (&player == p)
			return id;
	}

	return std::nullopt;
}

std::optional<sf::Vector2f> TestLobbyScene::definePlayerViewPosition(uint32_t playerId) const
{
	if (!m_lobbyStatePtr)
		return std::nullopt;

	auto seatPositionOpt = m_lobbyStatePtr->getPlayerSeatPosition(playerId);
	if (!seatPositionOpt)
		return std::nullopt;

	if (*seatPositionOpt == Constants::Lobby::QueueSeat)
	{
		auto indexInQueueOpt = m_lobbyStatePtr->getPlayerIndexInQueueById(playerId);
		if (!indexInQueueOpt)
			return std::nullopt;

		return queueLayout.getPositionByIndex(*indexInQueueOpt);
	}

	else
		return *tableLayout.getPosition(*seatPositionOpt);


	return std::nullopt;
}


void TestLobbyScene::movePlayerWithAnimation(uint32_t playerId, int seatPosition)
{
	auto player = playerById.at(playerId);
	assert(player);

	if (seatPosition == Constants::Lobby::QueueSeat)
	{
		auto indexOpt = m_lobbyStatePtr->getPlayerIndexInQueueById(playerId);

		auto& animation = player->addAnimation<EaseInOutCubicMoveAnimation>([player](sf::Vector2f pos) { player->setPosition(pos); },
			player->getPosition(), queueLayout.getPositionByIndex(*indexOpt), Constants::Animations::StandardMoveAnimationTime);

		player->blockHovered();
		animation.setOnFinish([player]() { player->unblockHovered(); });
	}

	else
	{
		auto& animation = player->addAnimation<EaseInOutCubicMoveAnimation>([player](sf::Vector2f pos) { player->setPosition(pos); },
			player->getPosition(), *tableLayout.getPosition(seatPosition), Constants::Animations::StandardMoveAnimationTime);

		player->blockHovered();
		animation.setOnFinish([player]() { player->unblockHovered(); });

		animateQueueAfterRemoval();
	}
}

void TestLobbyScene::animateQueueAfterRemoval()
{
	const auto& playerIds = m_lobbyStatePtr->getAllPlayersIdsInQueue();

	for (uint32_t i = 0; i < playerIds.size(); ++i)
	{
		auto player = playerById.at(playerIds[i]);
		auto currentPlayerPosition = player->getPosition();

		if (currentPlayerPosition == queueLayout.getPositionByIndex(i))
			continue;
		else
		{
			auto& animation = player->addAnimation<EaseInOutCubicMoveAnimation>([player](sf::Vector2f pos) { player->setPosition(pos); },
				currentPlayerPosition, queueLayout.getPositionByIndex(i), Constants::Animations::StandardMoveAnimationTime);

			player->blockHovered();
			animation.setOnFinish([player]() { player->unblockHovered(); });
		}
	}
}

template <typename AnimationType>
void TestLobbyScene::addPlayerReturnAnimation(uint32_t playerId, float duration)
{
	auto player = playerById.at(playerId);
	if (!player)
		throw std::runtime_error("Player do not exist");

	auto positionOpt = m_lobbyStatePtr->getPlayerSeatPosition(playerId);
	if (!positionOpt)
		throw std::runtime_error("Player do not have position");

	sf::Vector2f targetPosition;

	if (positionOpt.value() == Constants::Lobby::QueueSeat)
		targetPosition = queueLayout.getPositionByIndex(m_lobbyStatePtr->getPlayerIndexInQueueById(playerId).value());
	else
		targetPosition = *tableLayout.getPosition(positionOpt.value());


	AnimationType& animation = player->addAnimation<AnimationType>([player](sf::Vector2f pos) { player->setPosition(pos); },
		player->getPosition(), targetPosition, duration);

	player->blockHovered();
	animation.setOnFinish([player]() { player->unblockHovered(); });
}


void TestLobbyScene::onSnapshotAppliedEvent(const SnapshotAppliedEvent& event, const EventInitiator& initiator)
{
	if (auto ptr = std::get_if<const ClientLobbyState*>(&event.state))
	{
		m_lobbyStatePtr = *ptr;
		rebuildFullScene();
	}
}


void TestLobbyScene::onPlayerConnectedEvent(const PlayerConnectedEvent& event, const EventInitiator& initiator)
{
	auto it = playerById.find(event.playerId);
	assert(it == playerById.end());

	addPlayer(event.playerId, event.playerNickname, event.playerRole, event.playerSeatPosition);
}


void TestLobbyScene::onSeatPositionsChangedEvent(const SeatPositionsChangedEvent& event, const EventInitiator& initiator)
{
	auto it = playerById.find(event.playerId);
	assert(it->second);

	auto seatOpt = m_lobbyStatePtr->getPlayerSeatPosition(event.playerId);
	assert(seatOpt);
	assert(*seatOpt == event.newSeatPosition);

	movePlayerWithAnimation(event.playerId, event.newSeatPosition);
}

void TestLobbyScene::onSeatPositionsSwappedEvent(const SeatPositionsSwappedEvent& event, const EventInitiator& initiator)
{
	auto firstPlayerSeatPositionOpt = m_lobbyStatePtr->getPlayerSeatPosition(event.firstPlayerId);
	auto secondPlayerSeatPositionOpt = m_lobbyStatePtr->getPlayerSeatPosition(event.secondPlayerId);

	assert(firstPlayerSeatPositionOpt && secondPlayerSeatPositionOpt);
	assert(firstPlayerSeatPositionOpt != secondPlayerSeatPositionOpt);

	movePlayerWithAnimation(event.firstPlayerId, *firstPlayerSeatPositionOpt);
	movePlayerWithAnimation(event.secondPlayerId, *secondPlayerSeatPositionOpt);
}