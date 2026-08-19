#include "PacketFactory.h"


Packet PacketFactory::makeStartGame()
{
	Packet packet(PacketType::StartGame);
	//to do

	return packet; //заглушка пока что
}

Packet PacketFactory::makeEndGame()
{
	Packet packet(PacketType::EndGame);
	//to do

	return packet; //заглушка пока что
}


Packet PacketFactory::makeUpdateRequestPacket()
{
	Packet packet(PacketType::UpdateRequest);
	return packet;
}

Packet PacketFactory::makeSnapshotAckPacket()
{
	Packet packet(PacketType::SnapshotAck);
	return packet;
}


Packet PacketFactory::makeHelloPacket(const uint64_t token, const std::string& nickname)
{
	Packet packet(PacketType::Hello);
	packet.write(token);
	packet.write(nickname);

	return packet;
}

Packet PacketFactory::makeWelcomePacket(const uint32_t id, const std::string& nickname, ClientRole role, SessionPhase phase)
{
	Packet packet(PacketType::Welcome);
	packet.write(id);
	packet.write(nickname);
	packet.write(role);
	packet.write(phase);

	return packet;
}


Packet PacketFactory::makeNotifyPlayerConnectedPacket(uint32_t playerId, const std::string& nickname, ClientRole role, int seatPosition)
{
	Packet packet(PacketType::NotifyPlayerConnected);
	packet.write(playerId);
	packet.write(nickname);
	packet.write(role);
	packet.write(seatPosition);

	return packet;
}

Packet PacketFactory::makeNotifyPlayerReconnectedPacket(uint32_t playerId)
{
	Packet packet(PacketType::NotifyPlayerReconnected);
	packet.write(playerId);

	return packet;
}


Packet PacketFactory::makeServerDisconnectPacket(DisconnectReason reason)
{
	Packet packet(PacketType::ServerDisconnect);
	packet.write(reason);

	return packet;
}

Packet PacketFactory::makePlayerDisconnectedPacket()
{
	Packet packet(PacketType::PlayerDisconnected);

	return packet;
}

Packet PacketFactory::makeNotifyPlayerDisconnectedPacket(uint32_t playerId, DisconnectReason reason)
{
	Packet packet(PacketType::NotifyPlayerDisconnected);
	packet.write(playerId);
	packet.write(reason);

	return packet;
}


Packet PacketFactory::makeNotifyPlayerNicknameChangedPacket(const uint32_t playerId, const std::string nickname)
{
	Packet packet(PacketType::NotifyPlayerNicknameChanged);
	packet.write(playerId);
	packet.write(nickname);

	return packet;
}


Packet PacketFactory::makeNotifyConnectionWithPlayerLostPacket(uint32_t playerId)
{
	Packet packet(PacketType::NotifyConnectionWithPlayerLost);
	packet.write(playerId);

	return packet;
}


Packet PacketFactory::makeSeatPositionsChangeRequestPacket(uint32_t playerId, int newSeatPosition)
{
	Packet packet(PacketType::SeatPositionsChangeRequest);
	packet.write(playerId);
	packet.write(newSeatPosition);

	return packet;
}

Packet PacketFactory::makeSeatPositionsSwapRequestPacket(uint32_t firstPlayerId, uint32_t secondPlayerId)
{
	Packet packet(PacketType::SeatPositionsSwapRequest);
	packet.write(firstPlayerId);
	packet.write(secondPlayerId);

	return packet;
}

Packet PacketFactory::makeNotifySeatPositionsChangedPacket(uint32_t playerId, int seatPosition)
{
	Packet packet(PacketType::NotifySeatPositionsChanged);
	packet.write(playerId);
	packet.write(seatPosition);

	return packet;
}

Packet PacketFactory::makeNotifySeatPositionsSwappedPacket(uint32_t firstPlayerId, uint32_t secondPlayerId)
{
	Packet packet(PacketType::NotifySeatPositionsSwapped);
	packet.write(firstPlayerId);
	packet.write(secondPlayerId);

	return packet;
}


Packet PacketFactory::makePlayersPerGameChangeRequestPacket(uint32_t value)
{
	Packet packet(PacketType::PlayersPerGameChangeRequest);
	packet.write(value);

	return packet;
}

Packet PacketFactory::makeNotifyPlayersPerGameChangedPacket(uint32_t value)
{
	Packet packet(PacketType::NotifyPlayersPerGameChanged);
	packet.write(value);

	return packet;
}


Packet PacketFactory::makeDealCardPacket(uint32_t playerId, uint32_t cardId)
{
	Packet packet(PacketType::DealCard);
	packet.write(playerId);
	packet.write(cardId);

	return packet;
}

Packet PacketFactory::makeNotifyDealCardToOthersPacket(uint32_t playerId)
{
	Packet packet(PacketType::NotifyDealToOthers);
	packet.write(playerId);

	return packet;
}


Packet PacketFactory::makePlayerTossPacket(uint32_t cardId)
{
	Packet packet(PacketType::PlayerToss);
	packet.write(cardId);

	return packet;
}

Packet PacketFactory::makeNotifyPlayerTossedPacket(uint32_t playerId, uint32_t cardId, int tablePosition)
{
	Packet packet(PacketType::NotifyPlayerTossed);
	packet.write(playerId);
	packet.write(cardId);
	packet.write(tablePosition);

	return packet;
}


Packet PacketFactory::makePlayerBeatPacket(uint32_t cardId, int tablePosition)
{
	Packet packet(PacketType::PlayerBeat);
	packet.write(cardId);
	packet.write(tablePosition);

	return packet;
}

Packet PacketFactory::makeNotifyPlayerBeatPacket(uint32_t playerId, uint32_t cardId, int tablePosition)
{
	Packet packet(PacketType::NotifyPlayerBeat);
	packet.write(playerId);
	packet.write(cardId);
	packet.write(tablePosition);

	return packet;
}


Packet PacketFactory::makePlayerPassActionPacket(bool value)
{
	Packet packet(PacketType::PlayerPassAction);
	packet.write(value);

	return packet;
}

Packet PacketFactory::makeNotifyPlayerPassActionPacket(uint32_t playerId, bool value)
{
	Packet packet(PacketType::NotifyPassAction);
	packet.write(playerId);
	packet.write(value);

	return packet;
}


Packet PacketFactory::makePlayerWinnerPacket(uint32_t playerId, bool value)
{
	Packet packet(PacketType::PlayerWinner);
	packet.write(playerId);
	packet.write(value);

	return packet;
}

Packet PacketFactory::makePlayerLoserPacket(uint32_t playerId, bool value)
{
	Packet packet(PacketType::PlayerLoser);
	packet.write(playerId);
	packet.write(value);

	return packet;
}


Packet PacketFactory::makeShowCardPacket(uint32_t cardId)
{
	Packet packet(PacketType::ShowCard);
	packet.write(cardId);

	return packet;
}

Packet PacketFactory::makeNotifyIdentifiedTrumpCardPacket(uint32_t cardId)
{
	Packet packet(PacketType::NotifyIdentifiedTrumpCard);
	packet.write(cardId);

	return packet;
}


Packet PacketFactory::makeNotifyNewMainAttackerPlayerPacket(uint32_t playerId)
{
	Packet packet(PacketType::NotifyNewMainAttackerPlayer);
	packet.write(playerId);

	return packet;
}

Packet PacketFactory::makeNotifyNewDefenderPlayerPacket(uint32_t playerId)
{
	Packet packet(PacketType::NotifyNewMainAttackerPlayer);
	packet.write(playerId);

	return packet;
}


Packet PacketFactory::makeNotifyClearTablePacket(uint32_t destinationId, const std::vector<uint32_t>& cardIds)
{
	Packet packet(PacketType::NotifyClearTable);
	packet.write(destinationId);
	packet.write(cardIds.size());
	for (const auto& id : cardIds)
		packet.write(id);

	return packet;
}


Packet PacketFactory::makeNotifyNewGamePhasePacket(GameState::Phase phase)
{
	Packet packet(PacketType::NotifyNewPhase);
	packet.write(phase);

	return packet;
}



Packet PacketFactory::makeSnapshotBeginPacket(SnapshotType type)
{
	Packet packet(PacketType::SnapshotBegin);
	packet.write(type);

	return packet;
}

Packet PacketFactory::makeSnapshotEndPacket(SnapshotType type)
{
	Packet packet(PacketType::SnapshotEnd);
	packet.write(type);

	return packet;
}


Packet PacketFactory::makeSnapshotLobbySettingsPacket(uint32_t clientId, ClientRole clientRole, bool isLobbyOpen, bool canMoveToAnotherSeat, const RulesType rulesType, const uint32_t playersPerGame)
{
	Packet packet(PacketType::SnapshotLobbySettings);
	packet.write(clientId);
	packet.write(clientRole);
	packet.write(isLobbyOpen);
	packet.write(canMoveToAnotherSeat);
	packet.write(rulesType);
	packet.write(playersPerGame);
	
	return packet;
}

Packet PacketFactory::makeSnapshotLobbyPlayerInfoPacket(const uint32_t playerId, const std::string& playerNickname, ClientRole playerRole)
{
	Packet packet(PacketType::SnapshotLobbyPlayerInfo);
	packet.write(playerId);
	packet.write(playerNickname);
	packet.write(playerRole);

	return packet;
}

Packet PacketFactory::makeSnapshotLobbySeatPositionsPacket(const std::unordered_map<int32_t, uint32_t>& seatPositions)
{
	Packet packet(PacketType::SnapshotLobbySeatPositions);
	packet.write(static_cast<uint8_t>(seatPositions.size()));

	for (const auto& seat : seatPositions)
	{
		packet.write(seat.first);
		packet.write(seat.second);
	}

	return packet;
}

Packet PacketFactory::makeSnapshotLobbyQueuePositionsPacket(const std::vector<uint32_t>& queuePositions)
{
	Packet packet(PacketType::SnapshotLobbyQueuePositions);
	packet.write(static_cast<uint8_t>(queuePositions.size()));

	for (uint32_t i = 0; i < queuePositions.size(); ++i)
		packet.write(queuePositions[i]);

	return packet;
}
