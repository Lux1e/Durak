#pragma once
#include "../../core/Trackable.h"
#include "Panel.h"
#include "UIInteractive.h"
#include "../../game/GameContext.h"
#include "../../core/input/InputCapture.h"


class EventSystem;
class EventInitiator;



class ILayer : public Trackable
{
public:
	ILayer(GameContext& gameContext, InputCapture& inputCapture, sf::Vector2f size) : m_gameContext(gameContext), m_inputCapture(inputCapture), m_size(size)
	{
		m_isVisible = true;
		m_isModal = false;
	}

	virtual ~ILayer() = 0;

	virtual bool handleEvents(const sf::Event& event) { return false; };
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

	Panel& addPanel(std::unique_ptr<Panel> panel)
	{
		panel->setGameContext(m_gameContext);
		panel->setInputCapture(m_inputCapture);

		m_panels.push_back(std::move(panel));
		return *m_panels.back().get();
	}

	void erasePanel(Panel& panel)
	{
		m_panelsToDelete.emplace_back(&panel);
	}

	void cleanUp()
	{
		for (auto& panel : m_panels)
			panel->cleanUp();

		for (const auto& panelToDelete : m_panelsToDelete)
		{
			auto it = std::find_if(m_panels.begin(), m_panels.end(), [panelToDelete](const auto& panel)
				{
					return panel.get() == panelToDelete;
				});

			if (it != m_panels.end())
				m_panels.erase(it);
		}

		m_panelsToDelete.clear();
	}

	void setVisible(bool value)
	{
		if (value == m_isVisible)
			return;

		m_isVisible = value;
	}

	bool isVisible() const { return m_isVisible; }

	void setModal(bool value) { m_isModal = value; }
	bool isModal() const { return m_isModal; }

protected:
	GameContext& m_gameContext;
	InputCapture& m_inputCapture;

	sf::Vector2f m_size;
	bool m_isVisible;
	bool m_isModal;

	std::vector<std::unique_ptr<Panel>> m_panels;
	std::vector<Panel*> m_panelsToDelete;
};


inline ILayer::~ILayer() {};