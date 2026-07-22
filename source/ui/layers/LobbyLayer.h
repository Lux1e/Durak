#pragma once
#include "../core/ILayer.h"
#include "../controllers/DragController.h"
#include "../../protocol/ClientRole.h"
#include "../LobbyPlayerView.h"
#include "../../game/GameContext.h"


class ClientLobbyState;
struct ClientContext;
class Panel;
class Button;
class LobbyQueuePanel;
class LobbySeatsPanel;
class LobbyTopLeftPanel;
class LobbyTablePanel;
struct SeatPositionsChangedEvent;
struct SeatPositionsSwappedEvent;



class LobbyLayer : public ILayer
{
public:
	LobbyLayer(GameContext& gameContext, sf::Vector2f size, const ClientLobbyState& clientLobbyState, const ClientContext& context) : ILayer(gameContext, size), m_clientLobbyState(&clientLobbyState), m_clientContext(context)
	{
		init();
	}

	bool handleEvents(const sf::Event& event) override;
	void update(float dt) override;

	void addPlayer(uint32_t id, std::string nickname, ClientRole role, int seatIndex);
	void removePlayer(uint32_t id);
	void removePlayer(LobbyPlayerView& player);

private:
	void init();

	void initBackgroundPanel(sf::Vector2f backgroundOffset);
	void initTopPanel();
	void initArrowRotatePanel();
	void initChatPanel();
	void initBottomButtonsPanel();
	void initSettingsPanel();
	void initPopupPanel();
	void initPlayersPanel();

	//FOR TESTS:
	/////////////////////////////////////////////////////////////////////////////////////////////
	void initTestPanel();
	Panel* m_testPanel = nullptr;
	/////////////////////////////////////////////////////////////////////////////////////////////

	void subscribeAll();


	const ClientLobbyState* m_clientLobbyState;
	const ClientContext& m_clientContext;

	DragController m_dragController;

	std::unordered_map<uint32_t, LobbyPlayerView*> m_playersById;

	Panel* m_backgroundPanel = nullptr;
	Panel* m_topPanel = nullptr;
	LobbyTopLeftPanel* m_topLeftPanel = nullptr;
	LobbyQueuePanel* m_queuePanel = nullptr;
	Panel* m_arrowRotatePanel = nullptr;
	LobbyTablePanel* m_tablePanel = nullptr;
	LobbySeatsPanel* m_seatsPanel = nullptr;
	Panel* m_chatPanel = nullptr;
	Panel* m_bottomButtonsPanel = nullptr;
	Panel* m_settingsPanel = nullptr;
	Panel* m_popupPanel = nullptr;
	Panel* m_playersPanel = nullptr;


	sf::Vector2f m_screenOffset;
	sf::Vector2f m_panelsOffset;

	sf::Vector2f m_lobbyPlayerViewGlobalSize;

	Button* m_readyButton;
	Button* m_startButton;


	void rebuildLayer();
	void resyncLayer();


	template <AnimationType T, typename... Args>
	T& applyMoveAnimationToPlayer(LobbyPlayerView& player, Args&&... args)
	{
		if (UIElement* element = m_dragController.getDraggingElement())
		{
			if (element == &player)
				m_dragController.endDragging();
		}

		player.setHovered(false);
		player.setPressed(false, false);

		player.blockDraggable();
		player.blockHovered();
		player.blockPressed();

		T& animation = player.addAnimation<T>(std::forward<Args>(args)...);
		animation.setOnFinish([playerPtr = &player]()
			{
				playerPtr->unblockHovered();
				playerPtr->unblockPressed();
				playerPtr->unblockDraggable();
			});

		return animation;
	}

	bool onLobbyPlayerViewDrop(LobbyPlayerView& player);

	bool canChangePosition();
	bool canSwapPositions();

	void updatePlayersSeatsInQueue();


	void onSeatPositionsChangedEvent(const SeatPositionsChangedEvent& event, const EventInitiator& initiator);
	void onSeatPositionsSwappedEvent(const SeatPositionsSwappedEvent& event, const EventInitiator& initiator);
};