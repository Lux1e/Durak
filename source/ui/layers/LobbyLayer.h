#pragma once
#include "../core/ILayer.h"
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
class LobbySettingsPanel;
struct PlayerConnectedEvent;
struct PlayerDisconnectedEvent;
struct SeatPositionsChangedEvent;
struct SeatPositionsSwappedEvent;
struct PlayersPerGameChangedEvent;
struct LobbyOpenStateChangedEvent;



class LobbyLayer : public ILayer
{
public:
	LobbyLayer(GameContext& gameContext, InputCapture& inputCapture, sf::Vector2f size, const ClientLobbyState& clientLobbyState, const ClientContext& context) :
		ILayer(gameContext, inputCapture, size), m_clientLobbyState(&clientLobbyState), m_clientContext(context)
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
	LobbySettingsPanel* m_settingsPanel = nullptr;
	Panel* m_popupPanel = nullptr;
	Panel* m_playersPanel = nullptr;


	sf::Vector2f m_screenOffset;
	sf::Vector2f m_panelsOffset;

	sf::Vector2f m_lobbyPlayerViewGlobalSize;

	Button* m_readyButton;
	Button* m_startButton;


	void rebuildLayer();
	void resyncLayer();

	void updatePlayersTablePositions();


	template <AnimationType T, typename... Args>
	T& applyMoveAnimationToPlayer(LobbyPlayerView& player, Args&&... args)
	{
		assert(!player.isDragged());

		player.blockInteraction();

		T& animation = player.addAnimation<T>(std::forward<Args>(args)...);
		animation.setOnFinish([playerPtr = &player]()
			{
				playerPtr->unblockInteraction();
			});

		return animation;
	}

	void onLobbyPlayerViewDrop(LobbyPlayerView& player);

	bool canChangePosition();
	bool canSwapPositions();

	void updatePlayersSeatsInQueue();


	void onPlayerConnectedEvent(const PlayerConnectedEvent& event, const EventInitiator& initiator);
	void onPlayerDisconnectedEvent(const PlayerDisconnectedEvent& event, const EventInitiator& initiator);

	void onSeatPositionsChangedEvent(const SeatPositionsChangedEvent& event, const EventInitiator& initiator);
	void onSeatPositionsSwappedEvent(const SeatPositionsSwappedEvent& event, const EventInitiator& initiator);

	void onPlayersPerGameChangedEvent(const PlayersPerGameChangedEvent& event, const EventInitiator& initiator);
	void onLobbyOpenStateChangedEvent(const LobbyOpenStateChangedEvent& event, const EventInitiator& initiator);
};