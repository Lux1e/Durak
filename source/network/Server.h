#pragma once
#include <SFML/Network.hpp>
#include <queue>
#include <memory>
#include "ServerPacketDispatcher.h"
#include "ClientSession.h"
#include "../core/Trackable.h"
#include "../core/EventSystem.h"
#include "../protocol/SessionPhase.h"
#include "../game/PlayerProfile.h"
#include "../game/LobbyLogic.h"
#include "../game/GameLogic.h"


class LobbyState;
class GameState;
class Packet;
class EventInitiator;



class Server final : public Trackable
{
public:
	Server() : packetDispatcher(*this)
	{
		init();
	}

	Server(const Server&) = delete;
	Server(const Server&&) = delete;
	Server& operator=(const Server&) = delete;
	Server& operator=(const Server&&) = delete;

	~Server() = default;


	void processAllReceivingPacket();
	void update(float dt);
	void processAllSendingPackets();
	void cleanUp();


	std::optional<ClientSession*> getClientSessionById(uint32_t playerId);
	std::optional<ClientSession*> getClientSessionByToken(uint64_t token);

	std::optional<PlayerProfile*> getPlayerProfileById(uint32_t playerId);
	std::optional<PlayerProfile*> getPlayerProfileByToken(uint64_t token);

private:
	sf::TcpListener listener;

	ServerPacketDispatcher packetDispatcher;
	EventSystem serverES;

	SessionPhase currentSessionPhase;

	std::unique_ptr<LobbyState> lobbyState;
	std::unique_ptr<GameState> gameState;

	LobbyLogic lobbyLogic;
	GameLogic gameLogic;


	std::vector<ClientSession> clients;
	std::deque<PlayerProfile> profiles;


	void init();
	void registerHandlers();
	void subscribeAll();


	void updateClients(float dt);

	void acceptNewClients();
	void eraseClients();

	void clientsToDisconnectByToken(uint64_t token);


	void broadcast(const Packet& packet, std::optional<uint32_t> optExcludePlayerId = std::nullopt);
	void broadcast(const std::vector<Packet>& packets, std::optional<uint32_t> optExcludePlayerId = std::nullopt);

	void processSendingPackets(ClientSession& client);
	void processReceivingPackets(ClientSession& client);


	bool isTokenCorrect(uint64_t token) const;
	bool isPlayerPresent(const PlayerProfile& profile) const;


	void sendFullSnapshotUpdate(ClientSession& client);


	void onHelloPacket(ClientSession& client, Packet& packet);
	void onPlayerDisconnectedPacket(ClientSession& client, Packet& packet);

	void onSnapshotAckPacket(ClientSession& client, Packet& packet);

	void onSeatPositionsChangeRequestPacket(ClientSession& client, Packet& packet);
	void onSeatPositionsSwapRequestPacket(ClientSession& client, Packet& packet);
	void onPlayersPerGameChangeRequestPacket(ClientSession& client, Packet& packet);

	void onPlayerTossPacket(ClientSession& client, Packet& packet);
	void onPlayerBeatPacket(ClientSession& client, Packet& packet);
	void onPlayerPassAction(ClientSession& client, Packet& packet);
};