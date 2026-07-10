#pragma once
#include "../core/ILayer.h"



class MainMenuLayer : public ILayer
{
public:
	MainMenuLayer(GameContext& gameContext, sf::Vector2f size) : ILayer(gameContext, size)
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