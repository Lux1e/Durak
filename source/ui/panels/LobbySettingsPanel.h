#pragma once
#include "../core/Panel.h"
#include "../widgets/Dropdown.h"


class ClientLobbyState;
struct GameContext;
struct ClientContext;



class LobbySettingsPanel : public Panel
{
public:
	LobbySettingsPanel(sf::Vector2f size = { 160.0f, 90.0f }, sf::Vector2f position = { 0.0f, 0.0f }) : Panel(size, position) {}

	void init(const ClientLobbyState& clientLobbyState, const GameContext& gameContext, const ClientContext& clientContext);

private:
	Panel* m_topSettingsPanel = nullptr;

	Dropdown m_gameModeDropdown;
	Dropdown m_gameSpeedDropdown;
	Dropdown m_deckDropdown;

	void initTopSettingsPanel(const ClientLobbyState& clientLobbyState, const GameContext& gameContext, const ClientContext& clientContext);

	void updateSettings(const ClientLobbyState& clientLobbyState, const ClientContext& clientContext);
};