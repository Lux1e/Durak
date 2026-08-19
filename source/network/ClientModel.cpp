#include <variant>
#include "ClientModel.h"
#include "PacketFactory.h"
#include "../protocol/SnapshotType.h"
#include "../game/events/domain/WelcomeEvent.h"
#include "../game/events/domain/PlayerConnectedEvent.h"
#include "../game/events/domain/ServerDisconnectEvent.h"
#include "../game/events/domain/PlayerReconnectedEvent.h"
#include "../game/events/domain/PlayerDisconnectedEvent.h"
#include "../game/events/domain/ConnectionWithPlayerLostEvent.h"
#include "../game/events/domain/PlayerNicknameChangedEvent.h"
#include "../game/events/domain/SeatPositionsChangedEvent.h"
#include "../game/events/domain/SeatPositionsSwappedEvent.h"
#include "../game/events/domain/PlayersPerGameChangedEvent.h"
#include "../game/events/domain/SnapshotAppliedEvent.h"
#include "../game/events/requests/SeatPositionsChangeRequestEvent.h"
#include "../game/events/requests/SeatPositionsSwapRequestEvent.h"
#include "../game/events/requests/PlayersPerGameChangeRequestEvent.h"
#include "../game/events/ui/LobbyStateSyncEvent.h"



void ClientModel::init()
{
	syncState = SyncState::Unsynced;
	initialized = false;

	registerHandlers();
	subscribeAll();

	clientLobbyState = nullptr;
	tempClientLobbyState = nullptr;

	statesToDelete.reserve(2);

	auto connectionState = client.connect({ 127,0,0,1 }, Constants::Network::PORT);
	if (connectionState == ConnectionState::Disconnected)
	{
		//Failed to connect Event
	}

	else
		client.sendPacket(PacketFactory::makeHelloPacket(token, localNickname));
}

void ClientModel::subscribeAll()
{
	ES.subscribe<SeatPositionsChangeRequestEvent, ClientModel>(this, &ClientModel::onSeatPositionsChangeRequestEvent);
	ES.subscribe<SeatPositionsSwapRequestEvent, ClientModel>(this, &ClientModel::onSeatPositionsSwapRequestEvent);
	ES.subscribe<PlayersPerGameChangeRequestEvent, ClientModel>(this, &ClientModel::onPlayersPerGameChangeRequestEvent);
}

void ClientModel::registerHandlers()
{
	commonPacketDispatcher.on(PacketType::ServerDisconnect, &ClientModel::onServerDisconnectPacket);
	commonPacketDispatcher.on(PacketType::SnapshotBegin, &ClientModel::onSnapshotBeginPacket);

	unsyncedPacketDispatcher.on(PacketType::Welcome, &ClientModel::onWelcomePacket);

	syncingPacketDispatcher.on(PacketType::SnapshotEnd, &ClientModel::onSnapshotEndPacket);
	syncingPacketDispatcher.on(PacketType::SnapshotLobbySettings, &ClientModel::onSnapshotLobbySettingsPacket);
	syncingPacketDispatcher.on(PacketType::SnapshotLobbyPlayerInfo, &ClientModel::onSnapshotLobbyPlayerInfoPacket);
	syncingPacketDispatcher.on(PacketType::SnapshotLobbySeatPositions, &ClientModel::onSnapshotLobbySeatPositionsPacket);
	syncingPacketDispatcher.on(PacketType::SnapshotLobbyQueuePositions, &ClientModel::onSnapshotLobbyQueuePositionsPacket);

	syncedPacketDispatcher.on(PacketType::NotifyPlayerConnected, &ClientModel::onNotifyPlayerConnectedPacket);
	syncedPacketDispatcher.on(PacketType::NotifyPlayerReconnected, &ClientModel::onNotifyPlayerReconnectedPacket);
	syncedPacketDispatcher.on(PacketType::NotifyPlayerDisconnected, &ClientModel::onNotifyPlayerDisconnectedPacket);
	syncedPacketDispatcher.on(PacketType::NotifyConnectionWithPlayerLost, &ClientModel::onNotifyConnectionWithPlayerLostPacket);
	syncedPacketDispatcher.on(PacketType::NotifyPlayerNicknameChanged, &ClientModel::onNotifyPlayerNicknameChangedPacket);
	syncedPacketDispatcher.on(PacketType::NotifySeatPositionsChanged, &ClientModel::onNotifySeatPositionsChangedPacket);
	syncedPacketDispatcher.on(PacketType::NotifySeatPositionsSwapped, &ClientModel::onNotifySeatPositionsSwappedPacket);
	syncedPacketDispatcher.on(PacketType::NotifyPlayersPerGameChanged, &ClientModel::onNotifyPlayersPerGameChangedPacket);
}


void ClientModel::processReceivingPackets()
{
	std::vector<Packet> packets = client.receivePackets();

	for (auto& p : packets)
	{
		if (!initialized)
		{
			if (p.getType() == PacketType::Welcome)
				unsyncedPacketDispatcher.dispatch(p);

			if (p.getType() == PacketType::ServerDisconnect)
				commonPacketDispatcher.dispatch(p);

			continue;
		}

		bool handled = false;

		switch (syncState)
		{
			case SyncState::Syncing:
				handled = syncingPacketDispatcher.dispatch(p);
				break;

			case SyncState::Synced:
				handled = syncedPacketDispatcher.dispatch(p);
				break;

			case SyncState::Unsynced:
				handled = unsyncedPacketDispatcher.dispatch(p);
				break;
		}

		if (!handled)
			commonPacketDispatcher.dispatch(p);
	}
}

void ClientModel::update(float dt)
{

}

void ClientModel::cleanUp()
{
	cleanStatesToDelete();
}


void ClientModel::processSendingPackets()
{
	client.sendPendingPackets();
}


uint64_t ClientModel::getToken() const
{
	return token;
}


void ClientModel::setLocalId(uint32_t id)
{
	localContext.localId = id;
}

uint32_t ClientModel::getLocalId() const
{
	return localContext.localId;
}


void ClientModel::setNickname(std::string& nickname)
{
	localNickname = nickname;
}

const std::string& ClientModel::getNickname() const
{
	return localNickname;
}


const ClientLobbyState* ClientModel::getClientLobbyState() const
{
	if (clientLobbyState)
		return clientLobbyState.get();

	return nullptr;
}


const ClientContext& ClientModel::getClientContext() const
{
	return localContext;
}


void ClientModel::cleanStatesToDelete()
{
	if (!statesToDelete.empty())
		statesToDelete.clear();
}


void ClientModel::sendSnapshotUpdateRequest()
{
	if (syncState == SyncState::Syncing)
		return;
	else
		syncState = SyncState::Syncing;

	client.sendPacket(PacketFactory::makeUpdateRequestPacket());
}


void ClientModel::onSnapshotBeginPacket(Packet& packet)
{
	if (syncState != SyncState::Syncing)
		syncState = SyncState::Syncing;

	SnapshotType type = packet.read<SnapshotType>();

	switch (type)
	{
		case SnapshotType::Lobby:
			tempClientLobbyState = std::make_unique<ClientLobbyState>();
			break;

		case SnapshotType::Game:
			break;
	}
}

void ClientModel::onSnapshotEndPacket(Packet& packet)
{
	syncState = SyncState::Synced;

	SnapshotType type = packet.read<SnapshotType>();

	using State =
		std::variant<
		const ClientLobbyState*,
		const ClientGameState*>;

	State state;

	switch (type)
	{
		case SnapshotType::Lobby:
			if (!tempClientLobbyState)
				return;

			statesToDelete.emplace_back(std::move(clientLobbyState));
			clientLobbyState = std::move(tempClientLobbyState);
			clientLobbyLogic.setClientLobbyState(*clientLobbyState);
			state = clientLobbyState.get();

			break;

		case SnapshotType::Game:
			return;
			break;
	}

	ES.publish<SnapshotAppliedEvent>(state);
	syncState = SyncState::Synced;
	client.sendPacket(PacketFactory::makeSnapshotAckPacket());
}


void ClientModel::onSnapshotLobbySettingsPacket(Packet& packet)
{
	if (!tempClientLobbyState)
		return;

	localContext.localId = packet.read<uint32_t>();
	localContext.localRole = packet.read<ClientRole>();
	tempClientLobbyState->setLobbyOpen(packet.read<bool>());
	tempClientLobbyState->setMoveToAnotherSeat(packet.read<bool>());
	tempClientLobbyState->setRulesType(packet.read<RulesType>());
	tempClientLobbyState->setPlayersPerGame(packet.read<uint32_t>());
}

void ClientModel::onSnapshotLobbyPlayerInfoPacket(Packet& packet)
{
	if (!tempClientLobbyState)
		return;

	uint32_t id = packet.read<uint32_t>();
	std::string nickname = packet.read<std::string>();
	ClientRole role = packet.read<ClientRole>();

	tempClientLobbyState->addClientPlayerData(ClientPlayerData(id, std::move(nickname), role));

	if (role == ClientRole::Host)
		tempClientLobbyState->setHostId(id);
}

void ClientModel::onSnapshotLobbySeatPositionsPacket(Packet& packet)
{
	if (!tempClientLobbyState)
		return;

	uint8_t size = packet.read<uint8_t>();

	for (uint8_t i = 0; i < size; ++i)
	{
		int seatPosition = packet.read<int32_t>();
		uint32_t playerId = packet.read<uint32_t>();
		tempClientLobbyState->assignPlayerToSeatPosition(seatPosition, playerId);
	}
}

void ClientModel::onSnapshotLobbyQueuePositionsPacket(Packet& packet)
{
	if (!tempClientLobbyState)
		return;

	uint8_t size = packet.read<uint8_t>();

	for (uint8_t i = 0; i < size; ++i)
	{
		uint32_t id = packet.read<uint32_t>();
		tempClientLobbyState->assignPlayerToSeatPosition(Constants::Lobby::QueueSeat, id);
	}
}


void ClientModel::onWelcomePacket(Packet& packet)
{
	localContext.localId = packet.read<uint32_t>();
	localNickname = packet.read<std::string>();
	localContext.localRole = packet.read<ClientRole>();
	currentSessionPhase = packet.read<SessionPhase>();

	if (currentSessionPhase == SessionPhase::LobbyPhase)
	{
		clientLobbyState = std::make_unique<ClientLobbyState>();
		clientLobbyLogic.setClientLobbyState(*clientLobbyState);
	}

	initialized = true;
	ES.publish<WelcomeEvent>({ currentSessionPhase, localContext, *clientLobbyState });
}


void ClientModel::onServerDisconnectPacket(Packet& packet)
{
	ES.publish<ServerDisconnectEvent>(packet.read<DisconnectReason>());
}


void ClientModel::onNotifyPlayerConnectedPacket(Packet& packet)
{
	uint32_t playerId = packet.read<uint32_t>();
	std::string nickname = packet.read<std::string>();
	ClientRole role = packet.read<ClientRole>();
	int seatPosition = packet.read<int32_t>();

	if (clientLobbyLogic.applyPlayerConnected(playerId, nickname, role, seatPosition))
		ES.publish<PlayerConnectedEvent>({ playerId, nickname, role, seatPosition });
	else
		sendSnapshotUpdateRequest();
}

void ClientModel::onNotifyPlayerReconnectedPacket(Packet& packet)
{
	uint32_t playerId = packet.read<uint32_t>();
	ES.publish<PlayerReconnectedEvent>({ playerId });
}

void ClientModel::onNotifyPlayerDisconnectedPacket(Packet& packet)
{
	uint32_t playerId = packet.read<uint32_t>();
	DisconnectReason reason = packet.read<DisconnectReason>();

	clientLobbyLogic.applyPlayerDisconnected(playerId);
	ES.publish<PlayerDisconnectedEvent>({ playerId, reason });
}


void ClientModel::onNotifyConnectionWithPlayerLostPacket(Packet& packet)
{
	ES.publish<ConnectionWithPlayerLostEvent>(packet.read<uint32_t>());
}


void ClientModel::onNotifyPlayerNicknameChangedPacket(Packet& packet)
{
	uint32_t playerId = packet.read<uint32_t>();
	const std::string nickname = packet.read<std::string>();

	if (clientLobbyLogic.applyPlayerNicknameChanged(playerId, nickname))
		ES.publish<PlayerNicknameChangedEvent>({ nickname }, playerId);
	else
		sendSnapshotUpdateRequest();
}


void ClientModel::onNotifySeatPositionsChangedPacket(Packet& packet)
{
	uint32_t playerId = packet.read<uint32_t>();
	int seatPosition = packet.read<int32_t>();

	if (clientLobbyLogic.applySeatPositionsChanged(playerId, seatPosition))
		ES.publish<SeatPositionsChangedEvent>({ playerId, seatPosition });
	else
		sendSnapshotUpdateRequest();
}

void ClientModel::onNotifySeatPositionsSwappedPacket(Packet& packet)
{
	uint32_t firstPlayerId = packet.read<uint32_t>();
	uint32_t secondPlayerId = packet.read<uint32_t>();

	if (clientLobbyLogic.applySeatPositionsSwapped(firstPlayerId, secondPlayerId))
		ES.publish<SeatPositionsSwappedEvent>({ firstPlayerId, secondPlayerId });
	else
		sendSnapshotUpdateRequest();
}


void ClientModel::onNotifyPlayersPerGameChangedPacket(Packet& packet)
{
	uint32_t playersPerGame = packet.read<uint32_t>();

	std::vector<SeatChange> vec = clientLobbyLogic.applyPlayersPerGameChange(playersPerGame);

	assert(vec.empty());

	//if (!vec.empty())
	//	for (auto it : vec)
	//		ES.publish<SeatPositionsChangedEvent>({ it.playerId, it.toSeatIndex });

	ES.publish<PlayersPerGameChangedEvent>(playersPerGame);
}


void ClientModel::onSeatPositionsChangeRequestEvent(const SeatPositionsChangeRequestEvent& event, const EventInitiator& initiator)
{
	uint32_t initiatorId = initiator.getInitiator().value();
	auto seatChangeOpt = clientLobbyLogic.applySeatPositionsChangeRequest(event.id, event.newSeatIndex, initiatorId);

	if (seatChangeOpt)
	{
		ES.publish<SeatPositionsChangedEvent>({ seatChangeOpt->playerId, seatChangeOpt->toSeatIndex }, initiatorId);
		client.sendPacket(PacketFactory::makeSeatPositionsChangeRequestPacket(seatChangeOpt->playerId, seatChangeOpt->toSeatIndex));
	}

	else
		ES.publish<LobbyStateSyncEvent>(localContext);
}

void ClientModel::onSeatPositionsSwapRequestEvent(const SeatPositionsSwapRequestEvent& event, const EventInitiator& initiator)
{
	uint32_t initiatorId = initiator.getInitiator().value();
	auto seatPositions = clientLobbyLogic.applySeatPositionsSwapRequest(event.firstPlayerId, event.secondPlayerId, initiatorId);

	if (seatPositions.empty())
		ES.publish<LobbyStateSyncEvent>(localContext);
	else
	{
		ES.publish<SeatPositionsSwappedEvent>({ event.firstPlayerId, event.secondPlayerId }, initiatorId);
		client.sendPacket(PacketFactory::makeSeatPositionsSwapRequestPacket(event.firstPlayerId, event.secondPlayerId));
	}
}


void ClientModel::onPlayersPerGameChangeRequestEvent(const PlayersPerGameChangeRequestEvent& event, const EventInitiator& initiator)
{
	auto seatPositions = clientLobbyLogic.applyPlayersPerGameChange(event.playersPerGameValue);

	for (const auto& seat : seatPositions)
	{
		ES.publish<SeatPositionsChangedEvent>({ seat.playerId, seat.toSeatIndex });
		client.sendPacket(PacketFactory::makeSeatPositionsChangeRequestPacket(seat.playerId, seat.toSeatIndex));
	}

	ES.publish<PlayersPerGameChangedEvent>(event.playersPerGameValue);
	client.sendPacket(PacketFactory::makePlayersPerGameChangeRequestPacket(event.playersPerGameValue));
}