#include "ClientSession.h"
#include "../core/Constants.h"
#include "Packet.h"
#include <iostream>



void ClientSession::init()
{
	bufferStartOffset = 0;

	playerProfile = nullptr;

	connectionState = ConnectionState::Connecting;
	syncState = SyncState::Unsynced;

	initialized = false;

	disconnectRequested = false;
	disconnectTimer = 0.0f;
}


void ClientSession::update(float dt)
{
	if (disconnectRequested)
		updateDisconnectTimer(dt);
}


void ClientSession::setPlayerProfile(PlayerProfile* profile)
{
	playerProfile = profile;
}

std::optional<PlayerProfile*> ClientSession::getPlayerProfile() const
{
	if (playerProfile)
		return playerProfile;
	else
		return std::nullopt;
}


bool ClientSession::isMarkedToDisconnect() const
{
	return disconnectRequested;
}

void ClientSession::requestDisconnect()
{
	disconnectRequested = true;
}

void ClientSession::cancelDisconnect()
{
	disconnectRequested = false;
	disconnectTimer = 0.0f;
}


float ClientSession::getDisconnectTimer() const
{
	return disconnectTimer;
}

void ClientSession::updateDisconnectTimer(float dt)
{
	disconnectTimer += dt;
}

void ClientSession::setDisconnectTimer(float value)
{
	disconnectTimer = value;
}



void ClientSession::sendPacket(const Packet& packet)
{
	PendingPacket pending;
	auto serialized = packet.serialize();
	///////////////////
	std::cout << "[SERVER] SEND TYPE " << static_cast<int>(serialized[4]) << " [ ";
	for (uint8_t& byte : serialized)
		std::cout << static_cast<int>(byte) << " ";
	std::cout << "]" << std::endl;
	//////////////////
	pending.data.reserve(serialized.size());
	pending.data = std::move(serialized);
	pending.sent = 0;
	outgoing.push(std::move(pending));
}

void ClientSession::sendPendingPackets()
{
	while (!outgoing.empty())
	{
		auto& pending = outgoing.front();
		std::size_t sentBytes;

		sf::Socket::Status status = socket->send(pending.data.data() + pending.sent, pending.data.size() - pending.sent, sentBytes);

		if (status == sf::Socket::Status::Done)
		{
			pending.sent += sentBytes;
			if (pending.sent >= pending.data.size())
				outgoing.pop();

			continue;
		}

		else if (status == sf::Socket::Status::Partial)
		{
			pending.sent += sentBytes;
			return;
		}

		else if (status == sf::Socket::Status::NotReady)
		{
			return;
		}

		else
		{
			if (connectionState != ConnectionState::Disconnected)
				connectionState = ConnectionState::Disconnected;

			return;
		}
	}
}

std::vector<Packet> ClientSession::receivePackets()
{
	std::vector<Packet> packets;

	while (true)
	{
		uint8_t tempBuffer[1024];
		std::size_t received;

		if (socket->receive(tempBuffer, sizeof(tempBuffer), received) != sf::TcpSocket::Status::Done)
			break;

		buffer.insert(buffer.end(), tempBuffer, tempBuffer + received);

		while (buffer.size() - bufferStartOffset >= 4)
		{
			int32_t bodyLen =
				(static_cast<int32_t>(buffer[bufferStartOffset + 0]) << 24) |
				(static_cast<int32_t>(buffer[bufferStartOffset + 1]) << 16) |
				(static_cast<int32_t>(buffer[bufferStartOffset + 2]) << 8) |
				(static_cast<int32_t>(buffer[bufferStartOffset + 3]));

			if (buffer.size() - bufferStartOffset < bodyLen + 4)
				break;

			std::vector<uint8_t> packetData(buffer.begin() + bufferStartOffset, buffer.begin() + bufferStartOffset + 4 + bodyLen);
			////////////////////////////////
			std::cout << "[SERVER] RECV TYPE: " << static_cast<int>(packetData[4]) << " [ ";
			for (uint8_t& byte : packetData)
				std::cout << static_cast<int>(byte) << " ";
			std::cout << "]" << std::endl;
			////////////////////////////////
			packets.push_back(Packet(std::move(packetData)));

			if (connectionState == ConnectionState::Connecting && packets.back().getType() == PacketType::Hello)
				connectionState = ConnectionState::Ready;

			bufferStartOffset += 4 + bodyLen;
		}

		if (bufferStartOffset > 1024)
		{
			buffer.erase(buffer.begin(), buffer.begin() + bufferStartOffset);
			bufferStartOffset = 0;
		}
	}

	return packets;
}

const std::queue<PendingPacket>& ClientSession::getOutgoingData() const
{
	return outgoing;
}


ConnectionState ClientSession::getConnectionState() const
{
	return connectionState;
}


SyncState ClientSession::getSyncState() const
{
	return syncState;
}

void ClientSession::startSync()
{
	if (syncState == SyncState::Syncing)
		return;

	syncState = SyncState::Syncing;
}

void ClientSession::endSync()
{
	if (syncState == SyncState::Synced)
		return;

	syncState = SyncState::Synced;
}


bool ClientSession::isInitialized() const
{
	return initialized;
}

void ClientSession::setInitialized(bool value)
{
	initialized = value;
}