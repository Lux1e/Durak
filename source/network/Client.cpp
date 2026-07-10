#include "Client.h"
#include "../network/PacketFactory.h"
#include <iostream>



void Client::init()
{
	socket.setBlocking(false);
	bufferStartOffset = 0;

	connectionState = ConnectionState::Disconnected;
}


ConnectionState Client::connect(const sf::IpAddress& ip, unsigned short port)
{
	auto result = socket.connect(ip, port);
	socket.setBlocking(false);

	if (result == sf::Socket::Status::Done || result == sf::Socket::Status::NotReady)
		connectionState = ConnectionState::Connecting;
	else
		connectionState = ConnectionState::Disconnected;

	return connectionState;
}


void Client::sendPacket(const Packet& packet)
{
	PendingPacket pending;
	pending.data = packet.serialize();
	///////////////////
	std::cout << "[CLIENT] SEND TYPE: " << static_cast<int>(pending.data[4]) << " [ ";
	for (uint8_t& byte : pending.data)
		std::cout << static_cast<int>(byte) << " ";
	std::cout << "]" << std::endl;
	//////////////////
	pending.sent = 0;

	outgoing.push(std::move(pending));
}

void Client::sendPendingPackets()
{
	while (!outgoing.empty())
	{
		auto& pending = outgoing.front();
		std::size_t sentBytes;

		sf::Socket::Status status = socket.send(pending.data.data() + pending.sent, pending.data.size() - pending.sent, sentBytes);

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

std::vector<Packet> Client::receivePackets()
{
	std::vector<Packet> packets;

	while (true)
	{
		uint8_t tempBuffer[1024];
		std::size_t received;

		if (socket.receive(tempBuffer, sizeof(tempBuffer), received) != sf::TcpSocket::Status::Done)
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
			std::cout << "[Client] RECV TYPE: " << static_cast<int>(packetData[4]) << " [ ";
			for (uint8_t& byte : packetData)
				std::cout << static_cast<int>(byte) << " ";
			std::cout << "]" << std::endl;
			////////////////////////////////
			packets.push_back(Packet(std::move(packetData)));

			if (connectionState == ConnectionState::Connecting && packets.back().getType() == PacketType::Welcome)
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

ConnectionState Client::getConnectionState() const
{
	return connectionState;
}