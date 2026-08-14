#pragma once
#include <SFML/Graphics.hpp>
#include "../../core/EventSystem.h"
#include "../../core/EventInitiator.h"
#include "../../game/GameContext.h"
#include "../../ui/core/ILayer.h"
#include "../../ui/widgets/Button.h"
#include "../input/InputCapture.h"



class IScene : public Trackable
{
public:
	IScene(GameContext& gameContext, InputCapture& inputCapture) : m_gameContext(gameContext), m_inputCapture(inputCapture)
	{
		m_size = { 0.0f,0.0f };
	}

	virtual ~IScene() = default;

	virtual void init(const sf::Vector2f& windowSize) = 0;

	void mouseInputUpdate()
	{
		if (m_inputCapture.active())
		{
			m_inputCapture.get()->inputUpdate(m_gameContext.input);
			return;
		}

		if (m_underMouseElement && m_underMouseElement->isPressed())
			return;

		auto* element = findUnderMouseElement();
		if (element != m_underMouseElement)
		{
			if (m_underMouseElement)
				m_underMouseElement->setHovered(false);

			if (auto* interactive = element->asInteractive())
			{
				interactive->setHovered(true);
				m_underMouseElement = interactive;
			}

			else
				m_underMouseElement = nullptr;
		}

		else
			if (!m_underMouseElement->isHovered())
				m_underMouseElement->setHovered(true);
	}

	void handleEvents(const sf::Event& event)
	{
		if (m_inputCapture.active())
		{
			m_inputCapture.get()->handleEvents(m_gameContext.input, event);
			return;
		}

		if (const auto* e = event.getIf<sf::Event::MouseButtonPressed>())
		{
			if (e->button == sf::Mouse::Button::Left)
				if (m_underMouseElement)
					m_underMouseElement->onMouseDown();

			return;
		}

		if (const auto* e = event.getIf<sf::Event::MouseButtonReleased>())
		{
			if (e->button == sf::Mouse::Button::Left)
				if (m_underMouseElement)
				{
					bool isHit = m_underMouseElement->hitTest(m_gameContext.input.mouseWorldPosition);
					m_underMouseElement->onMouseUp(isHit);
				}

			return;
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

	virtual void update(float dt)
	{
		for (auto& layer : m_layers)
			layer->update(dt);
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
	InputCapture& m_inputCapture;
	sf::Vector2f m_size;

	std::vector<std::unique_ptr<ILayer>> m_layers;
	std::vector<ILayer*> m_layersToDelete;

private:
	UIInteractive* m_underMouseElement = nullptr;

	UIElement* findUnderMouseElement()
	{
		for (auto it = m_layers.rbegin(); it != m_layers.rend(); ++it)
		{
			if (!it->get()->isVisible())
				continue;

			if (auto* element = it->get()->hitTest(m_gameContext.input.mouseWorldPosition))
				return element;

			if (it->get()->isModal())
				return nullptr;
		}

		return nullptr;
	}
};