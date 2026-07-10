#pragma once
#include "../../core/Trackable.h"
#include "Panel.h"
#include "UIInteractive.h"
#include "../../game/GameContext.h"


class EventSystem;
class EventInitiator;



class ILayer : public Trackable
{
public:
	ILayer(GameContext& gameContext, sf::Vector2f size) : m_gameContext(gameContext), m_size(size)
	{
		m_isVisible = true;
		m_isModal = false;
		m_underMouseElement = nullptr;
	}

	virtual ~ILayer() = 0;

	virtual void mouseInputUpdate()
	{
		if (!m_underMouseElement)
			return;

		if (!m_underMouseElement->isInteractive())
			return;

		auto& asInteractive = *m_underMouseElement->asInteractive();
		if (!asInteractive.isPressed())
			asInteractive.setHovered(true);
	}

	virtual bool handleEvents(const sf::Event& event)
	{
		if (const auto& e = event.getIf<sf::Event::MouseButtonPressed>())
		{
			if (e->button == sf::Mouse::Button::Left)
				return onMousePressedEvent();
		}

		if (const auto& e = event.getIf<sf::Event::MouseButtonReleased>())
		{
			if (e->button == sf::Mouse::Button::Left)
				return onMouseReleasedEvent();
		}

		return false;
	};

	virtual void update(float dt) {}

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states)
	{
		if (!m_isVisible) return;

		for (auto& panel : m_panels)
			panel->draw(target, states);
	}

	UIElement* hitTest(sf::Vector2f globalPoint)
	{
		if (isVisible())
		{
			for (auto it = m_panels.rbegin(); it != m_panels.rend(); ++it)
			{
				if (auto element = it->get()->hitTest(globalPoint))
					return element;
			}
		}

		return nullptr;
	}

	void setUnderMouseElement(UIElement& element)
	{
		m_underMouseElement = &element;
	}

	const UIElement* getUnderMouseElement() const
	{
		return m_underMouseElement;
	}

	virtual void underMouseEnd()
	{
		assert(m_underMouseElement);

		if (m_underMouseElement->isInteractive())
		{
			auto* interactivePtr = m_underMouseElement->asInteractive();
			interactivePtr->setHovered(false);
			interactivePtr->setPressed(false, false);
		}

		m_underMouseElement = nullptr;
	}

	Panel& addPanel(std::unique_ptr<Panel> panel) { m_panels.push_back(std::move(panel)); return *m_panels.back().get(); }

	void setVisible(bool value) { m_isVisible = value; }
	bool isVisible() const { return m_isVisible; }

	void setModal(bool value) { m_isModal = value; }
	bool isModal() const { return m_isModal; }

protected:
	GameContext& m_gameContext;

	sf::Vector2f m_size;
	bool m_isVisible;
	bool m_isModal;

	std::vector<std::unique_ptr<Panel>> m_panels;

	UIElement* m_underMouseElement;


	virtual bool onMousePressedEvent()
	{
		if (m_underMouseElement)
		{
			if (m_underMouseElement->isInteractive())
				m_underMouseElement->asInteractive()->setPressed(true);

			return true;
		}

		return false;
	}

	virtual bool onMouseReleasedEvent()
	{
		if (m_underMouseElement)
		{
			if (UIInteractive* interactive = m_underMouseElement->asInteractive(); interactive && interactive->isPressed())
			{
				bool isHit = m_underMouseElement->hitTest(m_gameContext.input.mouseWorldPosition);
				interactive->setPressed(false, isHit);

				if (isHit)
					interactive->onClick();
			}

			return true;
		}

		return false;
	}
};

inline ILayer::~ILayer() {};