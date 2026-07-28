#pragma once
#include <SFML/Graphics.hpp>
#include "../../core/EventSystem.h"
#include "../../core/EventInitiator.h"
#include "../../game/GameContext.h"
#include "../../ui/core/ILayer.h"
#include "../../ui/widgets/Button.h"



class IScene : public Trackable
{
public:
	IScene(GameContext& gameContext) : m_gameContext(gameContext)
	{
		m_size = { 0.0f,0.0f };

		m_activeLayer = nullptr;
		m_inputCaptured = false;
	}

	virtual ~IScene() = default;

	virtual void init(const sf::Vector2f& windowSize) = 0;
	virtual void mouseInputUpdate()
	{
		if (m_inputCaptured && m_activeLayer)
			m_activeLayer->mouseInputUpdate();
		else
		{
			auto found = false;

			for (auto it = m_layers.rbegin(); it != m_layers.rend(); ++it)
			{
				if (!it->get()->isVisible())
					continue;

				if (auto element = it->get()->hitTest(m_gameContext.input.mouseWorldPosition))
				{
					found = true;
					if (m_activeLayer && m_activeLayer->getUnderMouseElement() != element)
						m_activeLayer->underMouseEnd();

					m_activeLayer = it->get();
					m_activeLayer->setUnderMouseElement(*element);
					break;
				}

				if (it->get()->isModal())
					break;
			}

			if (!found && m_activeLayer)
			{
				m_activeLayer->underMouseEnd();
				m_activeLayer = nullptr;
			}

			if (m_activeLayer)
				m_activeLayer->mouseInputUpdate();
		}
	}

	virtual void update(float dt)
	{
		for (auto& layer : m_layers)
			layer->update(dt);
	}

	void handleEvents(const sf::Event& event)
	{
		if (auto e = event.getIf<sf::Event::MouseButtonPressed>())
		{
			if (e->button == sf::Mouse::Button::Left)
				m_inputCaptured = true;
		}

		if (auto e = event.getIf<sf::Event::MouseButtonReleased>())
		{
			if (e->button == sf::Mouse::Button::Left)
				m_inputCaptured = false;
		}

		for (auto it = m_layers.rbegin(); it != m_layers.rend(); ++it)
		{
			auto& layer = *it->get();
			if (layer.isVisible())
			{
				if (layer.handleEvents(event))
					return;

				if (layer.isModal())
					return;
			}
		}
	}

	void render(sf::RenderWindow& window, sf::RenderStates states)
	{
		for (auto& layer : m_layers)
			layer->draw(window, states);
	}

	ILayer& addLayer(std::unique_ptr<ILayer> layer)
	{
		m_layers.push_back(std::move(layer));
		return *m_layers.back().get();
	}

	void eraseLayer(ILayer& layer)
	{
		m_layersToDelete.emplace_back(&layer);
	}

	void cleanUp()
	{
		for (auto& layer : m_layers)
			layer->cleanUp();

		for (const auto& layerToDelete : m_layersToDelete)
		{
			auto it = std::find_if(m_layers.begin(), m_layers.end(), [layerToDelete](const auto& layer)
				{
					return layer.get() == layerToDelete;
				});

			if (it != m_layers.end())
				m_layers.erase(it);
		}

		m_layersToDelete.clear();
	}

protected:
	GameContext& m_gameContext;
	sf::Vector2f m_size;

	std::vector<std::unique_ptr<ILayer>> m_layers;
	std::vector<ILayer*> m_layersToDelete;

	virtual void subscribeAll() {}

	void setInputCaptured(bool value)
	{
		m_inputCaptured = value;
	}

private:
	ILayer* m_activeLayer;
	bool m_inputCaptured;
};