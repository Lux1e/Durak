#pragma once
#include "../core/Panel.h"
#include "../elements/RoundedRectangleElement.h"
#include "../../core/Constants.h"



class LobbyQueuePanel : public Panel
{
public:
	LobbyQueuePanel(sf::Vector2f size = { 160.0f, 90.0f }, sf::Vector2f position = { 0.0f, 0.0f }) : Panel(size, position) {}

	void init()
	{
		RoundedRectangleElement& queueBox = static_cast<RoundedRectangleElement&>(addChild(std::make_unique<RoundedRectangleElement>(m_size)));
		queueBox.shape().setFillColor(sf::Color::Black);
		queueBox.shape().setOutlineColor(sf::Color::White);
		queueBox.shape().setOutlineThickness(-2.0f);

		m_firstPlayerLocalSize = { m_size.x, m_size.y / static_cast<float>(Constants::Lobby::MaxPlayersInLobby) };
		m_firstPlayerPosition = { m_size.x * 0.5f, m_firstPlayerLocalSize.y * 0.5f };
	}


	sf::Vector2f getPlayerViewGlobalSize() const { return { getGlobalSize().x, getGlobalSize().y / Constants::Lobby::MaxPlayersInLobby }; }
	sf::Vector2f getPlayerGlobalSeatPositionByQueueIndex(uint32_t index) { return getGlobalTransform().transformPoint({ m_firstPlayerPosition.x, m_firstPlayerPosition.y + m_firstPlayerLocalSize.y * static_cast<float>(index) }); }

private:
	sf::Vector2f m_firstPlayerLocalSize;
	sf::Vector2f m_firstPlayerPosition;
};