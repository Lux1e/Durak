#pragma once
#include <SFML/Graphics.hpp>
#include "../../core/Trackable.h"
#include "../../core/scene/IScene.h"
#include "../../ui/widgets/Button.h"
#include "../../ui/layers/MainMenuLayer.h"


struct ExitRequestEvent;
struct InputIpAddressRequestEvent;



class TestMenuScene final : public IScene
{
public:
	TestMenuScene(GameContext& gameContext, InputCapture& inputCapture) : IScene(gameContext, inputCapture)
	{
		subscribeAll();
	}

	void init(const sf::Vector2f& windowSize) override;

private:
	void subscribeAll();

	ILayer* m_mainMenuLayer = nullptr;
	ILayer* m_settingsLayer = nullptr;
	ILayer* m_InputIpAddressLayer = nullptr;
	ILayer* m_popupLayer = nullptr;
	ILayer* m_exitLayer = nullptr;

	void onExitRequestEvent(const ExitRequestEvent& event, const EventInitiator& initiator);
	void onInputIpAddressRequestEvent(const InputIpAddressRequestEvent& event, const EventInitiator& initiator);
};