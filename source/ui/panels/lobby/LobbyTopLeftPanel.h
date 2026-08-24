#pragma once
#include "../../core/Panel.h"


class Button;
class ClientLobbyState;
struct GameContext;
struct ClientContext;



class LobbyTopLeftPanel : public Panel
{
public:
	LobbyTopLeftPanel(sf::Vector2f size = { 160.0f, 90.0f }, sf::Vector2f position = { 0.0f, 0.0f }) : Panel(size, position) {}


	void init(const ClientLobbyState& clientLobbyState, const GameContext& gameContext, const ClientContext& clientContext);
	void initLockButton(const ClientLobbyState& clientLobbyState, const GameContext& gameContext, const ClientContext& clientContext);

	void updateLockButton(const ClientLobbyState& clientLobbyState);

private:
	Button* m_lockButton = nullptr;

	sf::Color m_buttonsNormalColor;

	std::function<void(bool)> m_onUpdateLockButton;
};