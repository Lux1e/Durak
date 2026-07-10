#pragma once
#include "../state/InteractionColorsState.h"
#include "../state/OutlineSettingsState.h"



struct  VisualComponent
{
	InteractionColorsState colors;
	OutlineSettingsState outline;
	sf::Vector2f sizeFactor;
};