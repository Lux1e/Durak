#include "LobbyScene.h"
#include "../../ui/layers/LobbyLayer.h"
#include "../events/requests/ExitRequestEvent.h"
#include "../events/domain/SnapshotAppliedEvent.h"



void LobbyScene::init(const sf::Vector2f& windowSize)
{
	m_size = windowSize;

	m_lobbyLayer = &addLayer(std::make_unique<LobbyLayer>(m_gameContext, m_inputCapture, m_size, *m_lobbyState, m_localContext));
}


void LobbyScene::subscribeAll()
{

}

void LobbyScene::rebuildScene()
{
	init(m_size);
}

void LobbyScene::resyncScene()
{
	//to do
}


void LobbyScene::onExitRequestEvent(const ExitRequestEvent& event, const EventInitiator& initiator)
{
	m_inputCapture.release();

	if (m_exitLobbyLayer)
		m_exitLobbyLayer->setVisible(true);
}

void LobbyScene::onSnapshotAppliedEvent(const SnapshotAppliedEvent& event, const EventInitiator& initiator)
{
	if (auto ptr = std::get_if<const ClientLobbyState*>(&event.state))
	{
		m_lobbyState = *ptr;
		rebuildScene();
	}
}