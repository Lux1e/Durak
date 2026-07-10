#pragma once
#include "../core/ILayer.h"
#include <functional>



class ExitConfirmationLayer : public ILayer
{
public:
	ExitConfirmationLayer(GameContext& gameContext, sf::Vector2f size) : ILayer(gameContext, size)
	{
		init();
	}

	bool handleEvents(const sf::Event& event) override;

private:
	void init();

	void initOverlayPanel(Panel& backgroundPanel);
	void initExitConfirmationPanel(Panel& overlayPanel);

	Panel* m_overlayPanel = nullptr;
	Panel* m_exitConfirmationPanel = nullptr;
};