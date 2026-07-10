#pragma once
#include <SFML/Graphics.hpp>
#include "../../core/scene/IScene.h"
#include "../../core/Trackable.h"
#include "../../core/Constants.h"
#include "../../core/ClientContext.h"
#include "../../protocol/ClientRole.h"
#include "../../ui/primitives/RoundedRectangleShape.h"
#include "../../ui/QueueLayout.h"
#include "../../ui/TableLayout.h"
#include "../../ui/widgets/Label.h"
#include "../../ui/LobbyPlayerView.h"


class EventSystem;
class EventInitiator;
class ClientLobbyState;

struct SnapshotAppliedEvent;
struct PlayerConnectedEvent;
struct SeatPositionsChangedEvent;
struct SeatPositionsSwappedEvent;



//class TestLobbyScene final : public IScene, public Trackable
//{
//public:
//	TestLobbyScene(EventSystem& ES, const ClientContext& context, const ClientLobbyState& clientLobbyState) :
//		ES(ES), localContext(context), m_lobbyStatePtr(&clientLobbyState) {}
//
//	void init(const sf::Vector2f& windowSize) override;
//	void update(float dt, const InputState& input) override;
//	void handleEvents(const sf::Event& event) override;
//	void render(sf::RenderWindow& window, sf::RenderStates states) override;
//
//private:
//	EventSystem& ES;
//
//	const ClientLobbyState* m_lobbyStatePtr;
//	const ClientContext& localContext;
//
//	QueueLayout queueLayout;
//	TableLayout tableLayout;
//
//	std::vector<std::unique_ptr<LobbyPlayerView>> players;
//	std::unordered_map<uint32_t, LobbyPlayerView*> playerById;
//
//
//	sf::RectangleShape background;
//	RoundedRectangleShape roundedBackground;
//	RoundedRectangleShape topBox;
//	RoundedRectangleShape queueBox;
//	Label playersLabel;
//	RoundedRectangleShape tableBox;
//	RoundedRectangleShape table;
//	std::vector<RoundedRectangleShape> tableSeatBoxes;
//
//	sf::Vector2f playerViewBoxSize;
//
//	sf::Vector2f spacingFromRoundedBackgroundRatio;
//	float spacingUnderTopBoxRatio;
//	float spacingBetweenQueueBoxAndTableBoxRatio;
//
//	LobbyPlayerView* draggedPlayer;
//
//
//	void initQueueLayout();
//	void initTableLayout();
//	void initTableSeatBoxes();
//
//	void clearAllVisuals();
//	void rebuildFullScene();
//
//	void subscribeAll();
//
//	void drawAllPlayers(sf::RenderWindow& window, sf::RenderStates states);
//
//	void updatePlayers(float dt);
//
//
//	std::optional<int> hitTableSeatBox(sf::Vector2f point) const;
//
//	void addPlayer(uint32_t playerId, const std::string& nickname, ClientRole role, int seatPosition);
//	void deletePlayer(uint32_t id);
//
//	std::optional<uint32_t> getPlayerId(const LobbyPlayerView& player) const;
//	std::optional<sf::Vector2f> definePlayerViewPosition(uint32_t playerId) const;
//
//	void movePlayerWithAnimation(uint32_t playerId, int seatPosition);
//
//	void animateQueueAfterRemoval();
//	template <typename AnimationType>
//	void addPlayerReturnAnimation(uint32_t playerId, float duration);
//
//
//	void onSnapshotAppliedEvent(const SnapshotAppliedEvent& event, const EventInitiator& initiator);
//
//	void onPlayerConnectedEvent(const PlayerConnectedEvent& event, const EventInitiator& initiator);
//
//	void onSeatPositionsChangedEvent(const SeatPositionsChangedEvent& event, const EventInitiator& initiator);
//	void onSeatPositionsSwappedEvent(const SeatPositionsSwappedEvent& event, const EventInitiator& initiator);
//};