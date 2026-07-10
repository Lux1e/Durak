#pragma once
#include <variant>
#include "../Event.h"


class ClientLobbyState;
class ClientGameState;



struct SnapshotAppliedEvent final : public Event
{
	using StateVariant =
		std::variant <
		const ClientLobbyState*,
		const ClientGameState*>;

	SnapshotAppliedEvent(StateVariant state) : state(state) {}

	StateVariant state;
};