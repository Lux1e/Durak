#pragma once
#include <SFML/Network.hpp>
#include <queue>
#include "PendingPacket.h"
#include "../network/SendResult.h"
#include "ConnectionState.h"


class Packet;



class Client final
{
public:
	Client()
	{
		init();
	}

	ConnectionState connect(const sf::IpAddress& ip, unsigned short port);

	void sendPacket(const Packet& packet);
	void sendPendingPackets();

	std::vector<Packet> receivePackets();

	ConnectionState getConnectionState() const;

private:
	sf::TcpSocket socket;

	std::queue<PendingPacket> outgoing;
	std::vector<uint8_t> buffer;

	size_t bufferStartOffset;

	ConnectionState connectionState;


	void init();
};