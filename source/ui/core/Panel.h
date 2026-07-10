#pragma once
#include "UIElement.h"
#include "../../core/Trackable.h"



class Panel : public UIElement, public Trackable
{
public:
	Panel(sf::Vector2f size = { 160.0f, 90.0f }, sf::Vector2f position = { 0.0f, 0.0f }) : UIElement(size, position) {}
};