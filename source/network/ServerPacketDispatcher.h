#pragma once
#include <unordered_map>
#include "Packet.h"
#include "../protocol/PacketType.h"
#include "CLientSession.h"


class Server;


class ServerPacketDispatcher final
{
public:
	ServerPacketDispatcher(Server& server) : m_server(server) {}


	void on(PacketType type, void (Server::* methodPtr)(ClientSession& client, Packet& packet))
	{
		handlers[type] = methodPtr;
	}

	void dispatch(ClientSession& client, Packet& packet)
	{
		auto it = handlers.find(packet.getType());
		if (it != handlers.end())
		{
			(m_server.*it->second)(client, packet);
		}
	}

private:
	Server& m_server;
	std::unordered_map <PacketType, void(Server::*)(ClientSession&, Packet&)> handlers;
};