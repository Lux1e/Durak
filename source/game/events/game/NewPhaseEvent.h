#pragma once
#include "../Event.h"
#include "../../../states/server/GameState.h"



struct NewPhaseEvent final : public Event
{
	NewPhaseEvent(GameState::Phase phase) : phase(phase) {}

	GameState::Phase phase;
};