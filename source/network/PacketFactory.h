#pragma once
#include "Packet.h"
#include "../game/RulesType.h"
#include "../protocol/PacketType.h"
#include "../protocol/SnapshotType.h"
#include "../protocol/ClientRole.h"
#include "../protocol/SessionPhase.h"
#include "../protocol/DisconnectReason.h"
#include "../states/server/GameState.h"



class PacketFactory
{
public:
	static Packet makeStartGame();
	static Packet makeEndGame();

	static Packet makeUpdateRequestPacket();
	static Packet makeSnapshotAckPacket();

	static Packet makeHelloPacket(const uint64_t token, const std::string& nickname);

	static Packet makeWelcomePacket(const uint32_t id, const std::string& nickName, ClientRole role, SessionPhase phase);

	static Packet makeNotifyPlayerConnectedPacket(const uint32_t playerId, const std::string& nickname, ClientRole role, int seatPosition);
	static Packet makeNotifyPlayerReconnectedPacket(const uint32_t playerId);

	static Packet makeServerDisconnectPacket(DisconnectReason reason);
	static Packet makePlayerDisconnectedPacket();
	static Packet makeNotifyPlayerDisconnectedPacket(uint32_t playerId, DisconnectReason reason);

	static Packet makeNotifyPlayerNicknameChangedPacket(const uint32_t playerId, const std::string nickname);

	static Packet makeNotifyConnectionWithPlayerLostPacket(const uint32_t playerId);

	static Packet makeSeatPositionsChangeRequestPacket(uint32_t playerToMoveId, int newSeatPosition);
	static Packet makeSeatPositionsSwapRequestPacket(uint32_t firstPlayerId, uint32_t secondPlayerId);
	static Packet makeNotifySeatPositionsChangedPacket(uint32_t playerId, int seatPosition);
	static Packet makeNotifySeatPositionsSwappedPacket(uint32_t firstPlayerId, uint32_t secondPlayerId);

	static Packet makePlayersPerGameChangeRequestPacket(uint32_t value);
	static Packet makeNotifyPlayersPerGameChangedPacket(uint32_t value);

	static Packet makeLobbyOpenStateChangeRequestPacket(bool value);
	static Packet makeNotifyLobbyOpenStateChangedPacket(bool value);

	static Packet makeDealCardPacket(uint32_t playerId, uint32_t cardId);
	static Packet makeNotifyDealCardToOthersPacket(uint32_t playerId);

	static Packet makePlayerTossPacket(uint32_t cardId);
	static Packet makeNotifyPlayerTossedPacket(uint32_t playerId, uint32_t cardId, int tablePosition);

	static Packet makePlayerBeatPacket(uint32_t cardId, int tablePosition);
	static Packet makeNotifyPlayerBeatPacket(uint32_t playerId, uint32_t cardId, int tablePosition);

	static Packet makePlayerPassActionPacket(bool value);
	static Packet makeNotifyPlayerPassActionPacket(uint32_t playerId, bool value);

	static Packet makePlayerWinnerPacket(uint32_t playerId, bool value);
	static Packet makePlayerLoserPacket(uint32_t playerId, bool value);

	static Packet makeShowCardPacket(uint32_t cardId);
	static Packet makeNotifyIdentifiedTrumpCardPacket(uint32_t cardId);

	static Packet makeNotifyNewMainAttackerPlayerPacket(uint32_t playerId);
	static Packet makeNotifyNewDefenderPlayerPacket(uint32_t playerId);

	static Packet makeNotifyClearTablePacket(uint32_t destinationId, const std::vector<uint32_t>& cardIds);

	static Packet makeNotifyNewGamePhasePacket(GameState::Phase phase);


	//Snapshot Lobby Packets
	static Packet makeSnapshotBeginPacket(SnapshotType type);
	static Packet makeSnapshotEndPacket(SnapshotType type);

	static Packet makeSnapshotLobbySettingsPacket(uint32_t clientId, ClientRole clientRole, bool isLobbyOpen, bool canMoveToAnotherSeat, const RulesType rulesType, const uint32_t playersPerGame);
	static Packet makeSnapshotLobbyPlayerInfoPacket(const uint32_t playerId, const std::string& playerNickname, ClientRole playerRole);
	static Packet makeSnapshotLobbySeatPositionsPacket(const std::unordered_map<int32_t, uint32_t>& seatPositions);
	static Packet makeSnapshotLobbyQueuePositionsPacket(const std::vector<uint32_t>& queuePositions);
};