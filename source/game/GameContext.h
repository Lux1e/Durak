#pragma once


class EventSystem;
class TextureManager;
struct InputState;



struct GameContext
{
	GameContext(EventSystem& eventSystem, TextureManager& textureManager, const InputState& inputState) : ES(eventSystem), textureManager(textureManager), input(inputState) {}

	EventSystem& ES;
	TextureManager& textureManager;
	const InputState& input;
};