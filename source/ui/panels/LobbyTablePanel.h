#pragma once
#include "../core/Panel.h"


class ClientLobbyState;
class LobbySeatsPanel;
struct GameContext;
struct ClientContext;



class LobbyTablePanel : public Panel
{
public:
	LobbyTablePanel(sf::Vector2f size = { 160.0f, 90.0f }, sf::Vector2f position = { 0.0f, 0.0f }) : Panel(size, position) {}

	void init(const ClientLobbyState& clientLobbyState, const GameContext& gameContext, const ClientContext& clientContext, const sf::Vector2f& lobbyPlayerViewGlobalSize);
	LobbySeatsPanel* getLobbySeatsPanel();

private:
	LobbySeatsPanel* m_seatsPanel = nullptr;
};