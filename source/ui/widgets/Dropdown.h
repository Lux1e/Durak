#pragma once
#include "ListBox.h"
#include "../../game/GameContext.h"
#include "../../core/input/InputCapture.h"



class Dropdown
{
public:
	Dropdown(GameContext& gameContext) : m_gameContext(gameContext) {}
	Dropdown(GameContext& gameContext, UIInteractive& mainElement, ListBox& listBox) : m_gameContext(m_gameContext), m_mainElement(&mainElement), m_listBox(&listBox) {}


	void setMainElement(UIInteractive& mainElement)
	{
		m_mainElement = &mainElement;
		setInteractiveLogic();
	}

	void setListBox(ListBox& listBox)
	{
		m_listBox = &listBox;
		setInteractiveLogic();
	}

	UIInteractive* getMainElement()
	{
		assert(m_mainElement); //for tests
		return m_mainElement;
	}

	ListBox* getListBox()
	{
		assert(m_listBox); //for tests
		return m_listBox;
	}

	void setOnListBoxChildAction(std::function<void(UIInteractive&)> func) { m_listBoxChildAction = std::move(func); }

private:
	std::function<void(UIInteractive&)> m_listBoxChildAction;
	GameContext& m_gameContext;

	UIInteractive* m_mainElement = nullptr;
	ListBox* m_listBox = nullptr;

	void setInteractiveLogic()
	{
		if (!m_mainElement || !m_listBox)
			return;

		m_mainElement->setOnInputUpdate([this](UIInteractive&, const InputState& inputState)
			{
				auto* underMouseElement = m_listBox->hitTest(m_gameContext.input.mouseWorldPosition);
				if (underMouseElement)
				{
					if (auto* underMouseInteractive = underMouseElement->asInteractive())
						underMouseInteractive->setHovered(true);
				}

				for (auto& child : m_listBox->getAllChildren())
				{
					if (auto* interactive = child->asInteractive(); interactive && interactive->isHovered() && interactive != underMouseElement)
						interactive->setHovered(false);
				}
			});

		m_mainElement->setOnHandleEvents([this](UIInteractive&, const InputState& inputState, const sf::Event& event)
			{
				if (const auto* e = event.getIf<sf::Event::MouseButtonPressed>())
				{
					if (e->button == sf::Mouse::Button::Left)
					{
						if (auto* element = m_listBox->hitTest(m_gameContext.input.mouseWorldPosition))
						{
							if (auto* interactive = element->asInteractive())
								interactive->onMouseDown();
							return;
						}

						else if (m_mainElement->hitTest(m_gameContext.input.mouseWorldPosition))
						{
							m_mainElement->onMouseDown();
							return;
						}

						else
						{
							m_mainElement->releaseInput();
							return;
						}
					}

					return;
				}

				if (const auto* e = event.getIf<sf::Event::MouseButtonReleased>())
				{
					if (e->button == sf::Mouse::Button::Left)
					{
						if (auto* element = m_listBox->hitTest(m_gameContext.input.mouseWorldPosition))
						{
							if (auto* interactive = element->asInteractive())
								interactive->onMouseUp(true);
							return;
						}

						else
						{
							m_mainElement->onMouseUp(true);
							return;
						}
					}

					return;
				}
			});

		m_mainElement->setOnCaptureEnd([this](UIInteractive&)
			{
				m_listBox->setVisible(false);
			});

		m_mainElement->setOnMouseDown([this](UIInteractive&)
			{
				if (m_mainElement->isCaptured())
					m_mainElement->releaseInput();

				else
				{
					m_mainElement->captureInput();
					m_listBox->setVisible(true);
				}
			});

		m_listBox->setOnChildMouseUp([this](UIInteractive& element, bool isHit)
			{
				if (m_listBoxChildAction)
					m_listBoxChildAction(element);
				m_mainElement->releaseInput();
			});
	}
};