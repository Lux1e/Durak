#include "DragController.h"
#include "../../core/input/InputState.h"



void DragController::update(const InputState& inputState)
{
	if (!m_element)
		return;

	const UIElement* parent = m_element->getParent();

	sf::Transform inverse = parent ? parent->getGlobalTransform().getInverse() : sf::Transform::Identity;
	sf::Vector2f localDelta = inverse.transformPoint(inputState.mouseDeltaWorld) - inverse.transformPoint({ 0.0f, 0.0f });

	m_element->setPosition(m_element->getPosition() + localDelta);
}


bool DragController::isDragging() const
{
	return m_element;
}


void DragController::startDragging(UIElement* element)
{
	m_element = element;
}

void DragController::endDragging(std::optional<sf::Vector2f> globalPosition)
{
	if (m_onEndDragging)
		m_onEndDragging(globalPosition);

	m_element = nullptr;
}


void DragController::setOnEndDragging(std::function<void(std::optional<sf::Vector2f>)> func)
{
	m_onEndDragging = func;
}


UIElement* DragController::getDraggingElement()
{
	return m_element;
}
