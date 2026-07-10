#pragma once
#include <optional>
#include <vector>
#include <unordered_map>
#include "Packet.h"
#include "../protocol/PacketType.h"


class ClientModel;


class ClientPacketDispatcher final
{
public:
	ClientPacketDispatcher(ClientModel& client) : client(client) {}

	void on(PacketType type, void(ClientModel::* methodPtr)(Packet& packet))
	{
		handlers[type] = methodPtr;
	}

	bool dispatch(Packet& packet)
	{
		auto it = handlers.find(packet.getType());
		if (it != handlers.end())
		{
			(client.*it->second)(packet);
			return true;
		}

		return false;
	}

private:
	ClientModel& client;
	std::unordered_map<PacketType, void(ClientModel::*)(Packet&)> handlers;
};