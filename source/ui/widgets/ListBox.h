#pragma once
#include "../core/UIInteractive.h"



class ListBox final : public UIElement
{
public:
	ListBox(sf::Vector2f size = { 160.0f, 90.0f }, sf::Vector2f position = { 0.0f, 0.0f }) : UIElement(size, position)
	{
		setChildrenHitTest(true);
		m_currentY = 0.0f;
	}


	void setOnChildMouseDown(std::function<void(UIInteractive&)> func) { m_onChildMouseDown = std::move(func); }
	void setOnChildMouseUp(std::function<void(UIInteractive&, bool)> func) { m_onChildMouseUp = std::move(func); }
	void setOnChildClick(std::function<void(UIInteractive&)> func) { m_onChildClick = std::move(func); }

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
	std::function<void(UIInteractive&)> m_onChildMouseDown;
	std::function<void(UIInteractive&, bool)> m_onChildMouseUp;
	std::function<void(UIInteractive&)> m_onChildClick;


	void drawSelf(sf::RenderTarget& target, sf::RenderStates states) const override {}

	void onChildAdded(UIElement& child) override
	{
		recalculate();

		if (auto* interactive = child.asInteractive())
		{
			interactive->setOnMouseDown([this, interactive](UIInteractive&)
				{
					if (m_onChildMouseDown)
						m_onChildMouseDown(*interactive);
				});

			interactive->setOnMouseUp([this, interactive](UIInteractive&, bool isHit)
				{
					if (m_onChildMouseUp)
						m_onChildMouseUp(*interactive, isHit);
				});

			interactive->setOnClick([this, interactive]()
				{
					if (m_onChildClick)
						m_onChildClick(*interactive);
				});
		}
	}

	void onChildVisibilityChanged(UIElement& child) override
	{
		recalculate();
	}
};