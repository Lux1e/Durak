#pragma once
#include "Client.h"
#include "ClientPacketDispatcher.h"
#include "../core/Trackable.h"
#include "../core/EventSystem.h"
#include "../core/ClientContext.h"
#include "../protocol/ClientRole.h"
#include "../protocol/SessionPhase.h"
#include "../protocol/SyncState.h"
#include "../game/ClientLobbyLogic.h"
#include "../states/core/IState.h"
#include "../states/client/ClientLobbyState.h"


class EventSystem;
class EventInitiator;
class Packet;

struct SeatPositionsChangeRequestEvent;
struct SeatPositionsSwapRequestEvent;



class ClientModel final : public Trackable
{
public:
	ClientModel(EventSystem& eventSystem, const uint64_t& token, const std::string& nickname, ClientLobbyState& lobbyState) :
		commonPacketDispatcher(*this), unsyncedPacketDispatcher(*this), syncingPacketDispatcher(*this), syncedPacketDispatcher(*this),
		ES(eventSystem), token(token), localNickname(nickname)
	{
		init();
		clientLobbyLogic.setClientLobbyState(lobbyState);
	}


	void processReceivingPackets();
	void update(float dt);
	void cleanUp();
	void processSendingPackets();

	uint64_t getToken() const;
	
	void setLocalId(uint32_t id);
	uint32_t getLocalId() const;

	void setNickname(std::string& nickname);
	const std::string& getNickname() const;

	const ClientLobbyState* getClientLobbyState() const;
	const ClientContext& getClientContext() const;

private:
	SyncState syncState;

	ClientPacketDispatcher commonPacketDispatcher;
	ClientPacketDispatcher unsyncedPacketDispatcher;
	ClientPacketDispatcher syncingPacketDispatcher;
	ClientPacketDispatcher syncedPacketDispatcher;

	Client client;
	EventSystem& ES;
	SessionPhase currentSessionPhase;


	bool initialized;

	const uint64_t token;
	std::string localNickname;

	ClientContext localContext;

	ClientLobbyLogic clientLobbyLogic;

	std::unique_ptr<ClientLobbyState> clientLobbyState;
	std::unique_ptr<ClientLobbyState> tempClientLobbyState;

	std::vector<std::unique_ptr<IState>> statesToDelete;


	void init();
	void subscribeAll();
	void registerHandlers();

	void cleanStatesToDelete();

	void sendSnapshotUpdateRequest();


	//SnapshotsPackets
	void onSnapshotBeginPacket(Packet& packet);
	void onSnapshotEndPacket(Packet& packet);
	
	//LobbySnapshotPackets
	void onSnapshotLobbySettingsPacket(Packet& packet);
	void onSnapshotLobbyPlayerInfoPacket(Packet& packet);
	void onSnapshotLobbySeatPositionsPacket(Packet& packet);
	void onSnapshotLobbyQueuePositionsPacket(Packet& packet);


	void onWelcomePacket(Packet& packet);
	void onServerDisconnectPacket(Packet& packet);

	void onNotifyPlayerConnectedPacket(Packet& packet);
	void onNotifyPlayerReconnectedPacket(Packet& packet);
	void onNotifyPlayerDisconnectedPacket(Packet& packet);

	void onNotifyConnectionWithPlayerLostPacket(Packet& packet);

	void onNotifyPlayerNicknameChangedPacket(Packet& packet);

	void onNotifySeatPositionsChangedPacket(Packet& packet);
	void onNotifySeatPositionsSwappedPacket(Packet& packet);

	void onNotifyPlayersPerGameChangedPacket(Packet& packet);


	void onSeatPositionsChangeRequestEvent(const SeatPositionsChangeRequestEvent& event, const EventInitiator& initiator);
	void onSeatPositionsSwapRequestEvent(const SeatPositionsSwapRequestEvent& event, const EventInitiator& initiator);
};