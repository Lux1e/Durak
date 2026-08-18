#include "Server.h"
#include "../core/DebugLog.h"
#include "../core/Constants.h"
#include "Packet.h"
#include "PacketFactory.h"
#include "ClientModel.h"
#include "../game/GameLogic.h"
#include "../protocol/SnapshotType.h"
#include "../states/server/LobbyState.h"
#include "../states/server/GameState.h"



void Server::init()
{
	registerHandlers();
	subscribeAll();

	listener.listen(Constants::Network::PORT);
	listener.setBlocking(false);

	currentSessionPhase = SessionPhase::LobbyPhase;

	lobbyState = std::make_unique<LobbyState>();
	gameState = nullptr;

	lobbyLogic.setLobbyState(*lobbyState);
}

void Server::registerHandlers()
{
	packetDispatcher.on(PacketType::Hello, &Server::onHelloPacket);
	packetDispatcher.on(PacketType::PlayerDisconnected, &Server::onPlayerDisconnectedPacket);
	packetDispatcher.on(PacketType::SnapshotAck, &Server::onSnapshotAckPacket);
	packetDispatcher.on(PacketType::SeatPositionsChangeRequest, &Server::onSeatPositionsChangeRequestPacket);
	packetDispatcher.on(PacketType::SeatPositionsSwapRequest, &Server::onSeatPositionsSwapRequestPacket);
	packetDispatcher.on(PacketType::PlayersPerGameChangeRequest, &Server::onPlayersPerGameChangeRequestPacket);
}

void Server::subscribeAll()
{

}


void Server::update(float dt)
{
	acceptNewClients();
	updateClients(dt);
}

void Server::cleanUp()
{
	eraseClients();
}


void Server::updateClients(float dt)
{
	for (auto& client : clients)
	{
		if (client.getConnectionState() == ConnectionState::Disconnected && !client.isMarkedToDisconnect())
			client.requestDisconnect();

		client.update(dt);
	}
}


void Server::acceptNewClients()
{
	sf::TcpSocket socket;

	if (listener.accept(socket) == sf::TcpSocket::Status::Done)
	{
		socket.setBlocking(false);

		ClientSession client(std::make_unique<sf::TcpSocket>(std::move(socket)));
		clients.push_back(std::move(client));
	}
}


std::optional<ClientSession*> Server::getClientSessionById(uint32_t playerId)
{
	for (auto& client : clients)
	{
		if (auto optPlayerProfile = client.getPlayerProfile())
			if (optPlayerProfile.value()->getId() == playerId)
				return &client;
	}

	return std::nullopt;
}

std::optional<ClientSession*> Server::getClientSessionByToken(uint64_t token)
{
	for (auto& client : clients)
	{
		if (auto optPlayerProfile = client.getPlayerProfile())
			if (optPlayerProfile.value()->getToken() == token)
				return &client;
	}

	return std::nullopt;
}


void Server::eraseClients()
{
	std::erase_if(clients, [](const ClientSession& client)
		{
			return client.isMarkedToDisconnect() && (client.getOutgoingData().empty() || client.getDisconnectTimer() >= Constants::Timers::MaxDisconnectTime);
		});
}


void Server::clientsToDisconnectByToken(uint64_t token)
{
	for (auto& c : clients)
	{
		if (auto optPlayerProfile = c.getPlayerProfile())
		{
			optPlayerProfile.value()->getToken() == token;
			c.requestDisconnect();
		}
	}
}


std::optional<PlayerProfile*> Server::getPlayerProfileById(uint32_t playerId)
{
	for (auto& p : profiles)
	{
		if (playerId == p.getId())
			return &p;
	}

	return std::nullopt;
}

std::optional<PlayerProfile*> Server::getPlayerProfileByToken(uint64_t token)
{
	for (auto& p : profiles)
	{
		if (token == p.getToken())
			return &p;
	}

	return std::nullopt;
}


void Server::broadcast(const Packet& packet, std::optional<uint32_t> excludePlayerId)
{
	for (auto& client : clients)
	{
		if (excludePlayerId)
		{
			if (auto playerProfile = client.getPlayerProfile())
			{
				if (playerProfile.value()->getId() == *excludePlayerId)
					continue;
			}
		}

		client.sendPacket(packet);
	}
}

void Server::broadcast(const std::vector<Packet>& packets, std::optional<uint32_t> excludePlayerId)
{
	for (auto& client : clients)
	{
		if (excludePlayerId)
		{
			if (auto playerProfile = client.getPlayerProfile())
			{
				if (playerProfile.value()->getId() == *excludePlayerId)
					continue;
			}
		}

		for (auto& packet : packets)
			client.sendPacket(packet);
	}
}


void Server::processSendingPackets(ClientSession& client)
{
	client.sendPendingPackets();
}

void Server::processReceivingPackets(ClientSession& client)
{
	std::vector<Packet> packets = client.receivePackets();

	for (auto& packet : packets)
		packetDispatcher.dispatch(client, packet);
}


void Server::processAllSendingPackets()
{
	for (auto& client : clients)
		processSendingPackets(client);
}

void Server::processAllReceivingPacket()
{
	for (auto& client : clients)
		processReceivingPackets(client);
}


bool Server::isTokenCorrect(uint64_t token) const
{
	if (token < 0)
		return false;

	return true;
}

bool Server::isPlayerPresent(const PlayerProfile& profile) const
{
	switch (currentSessionPhase)
	{
		case SessionPhase::LobbyPhase:
			if (lobbyState->hasPlayer(profile))
				return true;

		case SessionPhase::GamePhase:
			if (gameState->hasPlayer(profile))
				return true;
	}

	return false;
}


void Server::sendFullSnapshotUpdate(ClientSession& client)
{
	auto playerProfileOpt = client.getPlayerProfile();
	assert(playerProfileOpt);

	if (client.getSyncState() == SyncState::Syncing)
		return;
	else
		client.startSync();

	switch (currentSessionPhase)
	{
		case SessionPhase::LobbyPhase:
			assert(lobbyState);
			client.sendPacket(PacketFactory::makeSnapshotBeginPacket(SnapshotType::Lobby));

			client.sendPacket(PacketFactory::makeSnapshotLobbySettingsPacket(client.getPlayerProfile().value()->getId(), client.getPlayerProfile().value()->getRole(),
				lobbyState->isLobbyOpen(), lobbyState->canMoveToAnotherSeat(), lobbyState->getRulesType(), lobbyState->getPlayersPerGame()));

			for (const auto* p : lobbyState->getPlayerProfilesInLobby())
				client.sendPacket(PacketFactory::makeSnapshotLobbyPlayerInfoPacket(p->getId(), p->getNickname(), p->getRole()));

			client.sendPacket(PacketFactory::makeSnapshotLobbySeatPositionsPacket(lobbyState->getSeatPositionsState().getAllPlayersIdsBySeatPositions()));
			client.sendPacket(PacketFactory::makeSnapshotLobbyQueuePositionsPacket(lobbyState->getPlayerIdsInQueue()));

			client.sendPacket(PacketFactory::makeSnapshotEndPacket(SnapshotType::Lobby));
			break;

		case SessionPhase::GamePhase:
			assert(gameState);
			client.sendPacket(PacketFactory::makeSnapshotBeginPacket(SnapshotType::Game));

			//...

			client.sendPacket(PacketFactory::makeSnapshotEndPacket(SnapshotType::Game));
			break;
	}
}


void Server::onHelloPacket(ClientSession& client, Packet& packet)
{
	uint64_t token = packet.read<uint64_t>();
	const std::string nickname = packet.read<std::string>();

	if (!isTokenCorrect(token))
	{
		client.sendPacket(PacketFactory::makeServerDisconnectPacket(DisconnectReason::InvalidToken));
		client.requestDisconnect();

		return;
	}

	if (auto optPlayerProfile = getPlayerProfileByToken(token))
	{
		clientsToDisconnectByToken(token);
		client.setPlayerProfile(optPlayerProfile.value());
	}

	else
	{
		profiles.emplace_back(token, nickname);
		client.setPlayerProfile(&profiles.back());
	}

	auto* playerProfile = client.getPlayerProfile().value();
	uint32_t playerId = playerProfile->getId();

	switch (currentSessionPhase)
	{
		case SessionPhase::LobbyPhase:
		{
			bool isReconnect = lobbyState->hasPlayer(*playerProfile);
			if (isReconnect)
			{
				auto playerRole = playerProfile->getRole();

				client.setInitialized(true);

				client.sendPacket(PacketFactory::makeWelcomePacket(playerId, nickname, playerRole, currentSessionPhase));
				sendFullSnapshotUpdate(client);

				broadcast(PacketFactory::makeNotifyPlayerConnectedPacket(playerId, playerProfile->getNickname(), playerRole, *lobbyState->getSeatPositionById(playerId)), playerId);

				if (nickname != playerProfile->getNickname())
				{
					playerProfile->setNickname(nickname);
					broadcast(PacketFactory::makeNotifyPlayerNicknameChangedPacket(playerId, nickname), playerId);
				}
			}

			else
			{
				auto reasonOpt = lobbyLogic.applyPlayerConnectRequest(*playerProfile);
				if (reasonOpt)
				{
					client.sendPacket(PacketFactory::makeServerDisconnectPacket(*reasonOpt));
					client.requestDisconnect();
				}

				else
				{
					auto playerRole = playerProfile->getRole();

					client.setInitialized(true);

					client.sendPacket(PacketFactory::makeWelcomePacket(playerId, nickname, playerRole, currentSessionPhase));
					sendFullSnapshotUpdate(client);

					broadcast(PacketFactory::makeNotifyPlayerConnectedPacket(playerId, nickname, playerRole, *lobbyState->getSeatPositionById(playerId)), playerId);
				}
			}

			break;
		}

		case SessionPhase::GamePhase:
			break;
	}
}


void Server::onPlayerDisconnectedPacket(ClientSession& client, Packet& packet)
{
	auto optClientProfile = client.getPlayerProfile();
	if (!optClientProfile)
		throw std::logic_error("ClientProfile do not exist");

	auto playerId = optClientProfile.value()->getId();

	switch (currentSessionPhase)
	{
		case SessionPhase::LobbyPhase:
			lobbyLogic.applyPlayerDisconnected(playerId);
			broadcast(PacketFactory::makeNotifyPlayerDisconnectedPacket(playerId, DisconnectReason::Quit), playerId);

			break;

		case SessionPhase::GamePhase:
			break;
	}

	client.requestDisconnect();
}

void Server::onSnapshotAckPacket(ClientSession& client, Packet& packet)
{
	client.endSync();
}


void Server::onSeatPositionsChangeRequestPacket(ClientSession& client, Packet& packet)
{
	uint32_t playerId = packet.read<uint32_t>();
	int seatPosition = packet.read<int>();
	uint32_t clientId = client.getPlayerProfile().value()->getId();

	if (currentSessionPhase == SessionPhase::LobbyPhase)
	{
		if (lobbyLogic.applySeatPositionsChangeRequest(playerId, seatPosition, clientId))
			broadcast(PacketFactory::makeNotifySeatPositionsChangedPacket(playerId, seatPosition), clientId);
		else
			sendFullSnapshotUpdate(client);
	}
}

void Server::onSeatPositionsSwapRequestPacket(ClientSession& client, Packet& packet)
{
	uint32_t firstPlayerId = packet.read<uint32_t>();
	uint32_t secondPlayerId = packet.read<uint32_t>();
	uint32_t clientId = client.getPlayerProfile().value()->getId();

	if (currentSessionPhase == SessionPhase::LobbyPhase)
	{
		if (lobbyLogic.applySeatPositionsSwapRequest(firstPlayerId, secondPlayerId, clientId))
			broadcast(PacketFactory::makeNotifySeatPositionsSwappedPacket(firstPlayerId, secondPlayerId), clientId);
		else
			sendFullSnapshotUpdate(client);
	}
}


void Server::onPlayersPerGameChangeRequestPacket(ClientSession& client, Packet& packet)
{
	uint32_t value = packet.read<uint32_t>();
	uint32_t clientId = client.getPlayerProfile().value()->getId();

	if (currentSessionPhase == SessionPhase::LobbyPhase)
	{
		if (lobbyLogic.applyPlayersPerGameChangeRequest(value, clientId))
			broadcast(PacketFactory::makeNotifyPlayersPerGameChangedPacket(value), clientId);
		else
			sendFullSnapshotUpdate(client);
	}
}