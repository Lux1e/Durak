#pragma once
#include <SFML/Network.hpp>
#include <memory>
#include <queue>
#include <optional>
#include "../network/PendingPacket.h"
#include "../game/PlayerProfile.h"
#include "../network/SendResult.h"
#include "ConnectionState.h"
#include "../protocol/SyncState.h"


class Packet;


class ClientSession final
{
public:
	explicit ClientSession(std::unique_ptr<sf::TcpSocket> socket) : socket(std::move(socket))
	{
		init();
	}

	ClientSession(ClientSession&&) = default;
	ClientSession& operator=(ClientSession&&) = default;

	ClientSession(const ClientSession&) = delete;
	ClientSession& operator=(const ClientSession&) = delete;


	~ClientSession()
	{
		if (socket)
			socket->disconnect();
	}


	void update(float dt);

	void setPlayerProfile(PlayerProfile* profile);
	std::optional<PlayerProfile*> getPlayerProfile() const;

	bool isMarkedToDisconnect() const;
	void requestDisconnect();
	void cancelDisconnect();

	void updateDisconnectTimer(float dt);

	float getDisconnectTimer() const;
	void setDisconnectTimer(float value);

	void sendPacket(const Packet& packet);
	void sendPendingPackets();

	std::vector<Packet> receivePackets();

	const std::queue<PendingPacket>& getOutgoingData() const;

	ConnectionState getConnectionState() const;

	SyncState getSyncState() const;
	void startSync();
	void endSync();

	bool isInitialized() const;
	void setInitialized(bool value);

private:
	std::unique_ptr<sf::TcpSocket> socket;

	std::vector<uint8_t> buffer;
	size_t bufferStartOffset;

	std::queue<PendingPacket> outgoing;

	ConnectionState connectionState;
	SyncState syncState;

	bool initialized;


	PlayerProfile* playerProfile;

	bool disconnectRequested;
	float disconnectTimer;

	void init();
};