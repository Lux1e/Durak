#pragma once
#include <SFML/Graphics.hpp>
#include <functional>
#include "../../core/Constants.h"
#include "../core/UIElement.h"



class DragController
{
public:
	DragController()
	{
		m_element = nullptr;
	}


	void update(const InputState& inputState);

	bool isDragging() const;

	void startDragging(UIElement* element);
	void endDragging(std::optional<sf::Vector2f> globalPosition = std::nullopt);

	void setOnEndDragging(std::function<void(std::optional<sf::Vector2f>)> func);

	UIElement* getDraggingElement();

private:
	UIElement* m_element;
	std::function<void(std::optional<sf::Vector2f>)> m_onEndDragging;
};