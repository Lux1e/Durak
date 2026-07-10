#pragma once
#include <cstdint>


enum class PacketType : uint8_t
{
	// ---- —ŒŒ¡Ÿ≈Õ»ﬂ Œ“ —≈–¬≈–¿ (1ñ127) ----
	Welcome = 1,

	NotifyPlayerConnected = 2,

	NotifyPlayerReconnected = 3,

	NotifyPlayerDisconnected = 4,
	ServerDisconnect = 5,

	NotifyConnectionWithPlayerLost = 6,

	NotifyPlayerNicknameChanged = 7,

	NotifySeatPositionsChanged = 8,
	NotifySeatPositionsSwapped = 9,
	NotifyPlayersPerGameChanged = 10,

	StartGame = 11,
	DealCard = 12,
	ShowCard = 13,
	NotifyIdentifiedTrumpCard = 14,
	NotifyNewPhase = 15,
	NotifyNewMainAttackerPlayer = 16,
	NotifyNewDefenderPlayer = 17,
	NotifyDealToOthers = 18,
	NotifyPlayerTossed = 19,
	NotifyPlayerBeat = 20,
	NotifyPassAction = 21,
	NotifyClearTable = 22,

	CardPlacedOnTable = 23, // ?
	PlayerWinner = 24,
	PlayerLoser = 25,
	EndGame = 26,

	//SnapshotsLobby
	SnapshotBegin = 80,
	SnapshotEnd = 81,
	SnapshotLobbySettings = 82,
	SnapshotLobbyPlayerInfo = 83,
	SnapshotLobbySeatPositions = 84,
	SnapshotLobbyQueuePositions = 85,

	// ---- —ŒŒ¡Ÿ≈Õ»ﬂ Œ“  À»≈Õ“¿ (128ñ255) ----
	Hello = 128,

	UpdateRequest = 129,
	SnapshotAck = 130,

	PlayerDisconnected = 131,

	SeatPositionsChangeRequest = 132,
	SeatPositionsSwapRequest = 133,
	PlayersPerGameChangeRequest = 134,

	PlayerToss = 135,
	PlayerBeat = 136,
	PlayerPassAction = 137
};