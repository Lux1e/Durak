#pragma once
#include "UIElement.h"
#include "../../core/Trackable.h"


struct GameContext;



class Panel : public UIElement, public Trackable
{
public:
	Panel(sf::Vector2f size = { 160.0f, 90.0f }, sf::Vector2f position = { 0.0f, 0.0f }) : UIElement(size, position) {}

	void setGameContext(GameContext& gameContext) { m_gameContext = &gameContext; }
	void setInputCapture(InputCapture& inputCapture) { m_inputCapture = &inputCapture; }

protected:
	GameContext* m_gameContext = nullptr;

private:
	void drawSelf(sf::RenderTarget& target, sf::RenderStates states) const override {}
};