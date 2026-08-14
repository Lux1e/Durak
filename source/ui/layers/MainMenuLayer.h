#pragma once
#include "../core/ILayer.h"



class MainMenuLayer : public ILayer
{
public:
	MainMenuLayer(GameContext& gameContext, InputCapture& inputCapture, sf::Vector2f size) : ILayer(gameContext, inputCapture, size)
	{
		init();
	}


	bool handleEvents(const sf::Event& event) override;

private:
	void init();
	
	void initBackgroundPanel(Panel& backgroundPanel);
	void initMainMenuButtonsPanel(Panel& mainMenuButtonsPanel);
	void initChooseMenuButtonsPanel(Panel& chooseMenuButtonPanel);

	Panel* m_backgroundPanel = nullptr;
	Panel* m_mainMenuButtonsPanel = nullptr;
	Panel* m_chooseMenuButtonsPanel = nullptr;
};