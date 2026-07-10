#pragma once
#include "../Event.h"
#include "../../../core/ClientContext.h"



struct LobbyStateSyncEvent final : public Event
{
	LobbyStateSyncEvent(const ClientContext& context) : clientContext(context) {}

	const ClientContext& clientContext;
};