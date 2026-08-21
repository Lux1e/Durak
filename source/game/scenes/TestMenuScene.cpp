#include "TestMenuScene.h"
#include "../../ui/layers/MainMenuLayer.h"
#include "../../ui/layers/InputIpAddressLayer.h"
#include "../../ui/layers/ExitConfirmationLayer.h"
#include "../events/requests/ExitRequestEvent.h"
#include "../events/requests/InputIpAddressRequestEvent.h"



void TestMenuScene::init(const sf::Vector2f& windowSize)
{
	m_size = windowSize;

	m_mainMenuLayer = &addLayer(std::make_unique<MainMenuLayer>(m_gameContext, m_inputCapture, m_size));
	m_InputIpAddressLayer = &addLayer(std::make_unique<InputIpAddressLayer>(m_gameContext, m_inputCapture, m_size));
	m_exitLayer = &addLayer(std::make_unique<ExitConfirmationLayer>(m_gameContext, m_inputCapture, m_size));
}


void TestMenuScene::subscribeAll()
{
	m_gameContext.ES.subscribe<ExitRequestEvent, TestMenuScene>(this, &TestMenuScene::onExitRequestEvent);
	m_gameContext.ES.subscribe<InputIpAddressRequestEvent, TestMenuScene>(this, &TestMenuScene::onInputIpAddressRequestEvent);
}


void TestMenuScene::onExitRequestEvent(const ExitRequestEvent& event, const EventInitiator& initiator)
{
	if (m_exitLayer)
		showModalLayer(*m_exitLayer);
}

void TestMenuScene::onInputIpAddressRequestEvent(const InputIpAddressRequestEvent& event, const EventInitiator& initiator)
{
	if (m_inputIpAddressLayer)
		showModalLayer(*m_inputIpAddressLayer);
}