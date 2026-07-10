#pragma once
#include "../Event.h"
#include "../../../protocol/DisconnectReason.h"


struct ServerDisconnectEvent final : public Event
{
	ServerDisconnectEvent(DisconnectReason reason) : reason(reason) {}

	DisconnectReason reason;
};