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
	NotifyLobbyOpenStateChanged = 11,

	StartGame = 12,
	DealCard = 13,
	ShowCard = 14,
	NotifyIdentifiedTrumpCard = 15,
	NotifyNewPhase = 16,
	NotifyNewMainAttackerPlayer = 17,
	NotifyNewDefenderPlayer = 18,
	NotifyDealToOthers = 19,
	NotifyPlayerTossed = 20,
	NotifyPlayerBeat = 21,
	NotifyPassAction = 22,
	NotifyClearTable = 23,

	CardPlacedOnTable = 24, // ?
	PlayerWinner = 25,
	PlayerLoser = 26,
	EndGame = 27,

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
	LobbyOpenStateChangeRequest = 135,

	PlayerToss = 136,
	PlayerBeat = 137,
	PlayerPassAction = 138
};