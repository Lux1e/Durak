#pragma once
#include <optional>


class EventInitiator
{
public:
	EventInitiator() = default;
	EventInitiator(std::optional<uint32_t> playerId) : initiator(std::move(playerId)) {}

	std::optional<uint32_t> getInitiator() const { return initiator; }

private:
	std::optional<uint32_t> initiator;
};