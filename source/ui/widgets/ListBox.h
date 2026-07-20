#pragma once
#include "../core/UIInteractive.h"



class ListBox final : public UIElement
{
public:
	ListBox(sf::Vector2f size = { 160.0f, 90.0f }, sf::Vector2f position = { 0.0f, 0.0f }) : UIElement(size, position)
	{
		m_currentY = 0.0f;
	}


	void setOnChildClick(std::function<void(UIElement&)> func) { m_onChildClick = std::move(func); }

	void recalculate()
	{
		m_currentY = 0.0f;

		for (auto& child : m_children)
		{
			if (child->isVisible())
			{
				child->setSize({ m_size.x, child->getSize().y });
				child->setPosition({ 0.0f, m_currentY });
				m_currentY += child->getSize().y;
			}
		}

		m_size.y = m_currentY;
	}

private:
	float m_currentY;
	std::function<void(UIElement&)> m_onChildClick;


	void drawSelf(sf::RenderTarget& target, sf::RenderStates states) const override {}

	void onChildAdded(UIElement& child) override
	{
		recalculate();

		if (auto* interactive = child.asInteractive())
		{
			interactive->setOnClick([this, &child]()
				{
					if (m_onChildClick)
						m_onChildClick(child);
				});
		}
	}

	void onChildVisibilityChanged(UIElement& child) override
	{
		recalculate();
	}
};