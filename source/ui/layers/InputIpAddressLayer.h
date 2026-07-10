#pragma once
#include "../core/ILayer.h"


class TextBox;
class Button;



class InputIpAddressLayer : public ILayer
{
public:
	InputIpAddressLayer(GameContext& gameContext, sf::Vector2f size) : ILayer(gameContext, size)
	{
		init();
	}

	bool handleEvents(const sf::Event& event) override;

private:
	void init();

	void initOverlayPanel(Panel& overlayPanel);
	void initIpPanel(Panel& ipPanel);

	Panel* m_overlayPanel = nullptr;
	Panel* m_ipPanel = nullptr;

	TextBox* m_ipBox = nullptr;
	Button* m_connectButton = nullptr;

	void updateIPBox();

	std::function<void(bool state)> switchConnectButtonColors;
};