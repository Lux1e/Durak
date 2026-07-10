#pragma once
#include "../Event.h"
#include "../../../core/ClientContext.h"
#include "../../../protocol/ClientRole.h"
#include "../../../protocol/SessionPhase.h"
#include "../../../states/core/IState.h"



struct WelcomeEvent final : public Event
{
	WelcomeEvent(SessionPhase currentPhase, const ClientContext& context, const IState& state) : phase(currentPhase), localContext(context), state(state) {}

	SessionPhase phase;
	const ClientContext& localContext;
	const IState& state;
};