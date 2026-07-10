#pragma once
#include <cstdint>
#include <SFML/Graphics.hpp>



namespace Constants
{
	namespace Lobby
	{
		constexpr int MaxPlayersInLobby = 8;

		constexpr uint32_t MaxPlayersPerGame = 4;
		constexpr uint32_t MinPlayersPerGame = 2;

		constexpr int QueueSeat = -1;
	}

	namespace Game
	{
		constexpr int TotalCardsInHands = 6;

		constexpr int DeckId = -10;
		constexpr int BoneyardId = -11;

		constexpr int MaxTablePositions = 6;
		constexpr int MaxCardsOnTablePosition = 2;
	}

	namespace Timers
	{
		constexpr float UpdateRequestDelayTime = 3.0f;

		constexpr float MaxDisconnectTime = 60.0f;


		constexpr float StandardDelayTime = 3.0f;

		constexpr float ShowingCardDelayTime = 3.0f;

		constexpr float DealDelayTime = 0.25f;
		constexpr float TossDelayTime = 0.5f;

		constexpr float StandardSwitchingPhaseDelayTime = 1.5f;
		constexpr float FastSwitchingPhaseDelayTime = 0.5f;

		constexpr float ClearTableDelayTime = 2.0f;

		constexpr float AwardingDelayTime = 2.0f;
	}

	namespace Animations
	{
		constexpr float StandardMoveAnimationTime = 0.3f;
		constexpr float StandardRotateAnimationTime = 0.3f;

		constexpr float FastMoveAnimationTime = 0.1f;
		constexpr float FastRotateAnimationTime = 0.1f;
	}

	namespace Network
	{
		constexpr unsigned short PORT = 50000;
	}

	namespace UI
	{
		constexpr sf::Color OverlayColor = { 45, 45, 45, 240 };
	}
}