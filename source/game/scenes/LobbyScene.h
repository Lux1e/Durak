#pragma once
#include "../../core/scene/IScene.h"
#include "../../core/ClientContext.h"


class ClientLobbyState;
struct ExitRequestEvent;
struct SnapshotAppliedEvent;



class LobbyScene : public IScene
{
public:
	LobbyScene(GameContext& gameContext, const ClientContext& context, const ClientLobbyState& clientLobbyState) : IScene(gameContext), m_localContext(context), m_lobbyState(&clientLobbyState)
	{
		subscribeAll();
	}


	void init(const sf::Vector2f& windowSize) override;

private:
	void subscribeAll() override;

	void rebuildScene();
	void resyncScene();

	const ClientLobbyState* m_lobbyState;
	const ClientContext& m_localContext;

	ILayer* m_lobbyLayer = nullptr;
	ILayer* m_settingsLayer = nullptr;
	ILayer* m_popupLayer = nullptr;
	ILayer* m_exitLobbyLayer = nullptr;

	void onExitRequestEvent(const ExitRequestEvent& event, const EventInitiator& initiator);
	void onSnapshotAppliedEvent(const SnapshotAppliedEvent& event, const EventInitiator& initiator);
};