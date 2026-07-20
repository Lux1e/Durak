#pragma once
#include "../../core/Constants.h"
#include "../core/Panel.h"
#include "../elements/RoundedRectangleElement.h"
#include "../elements/RectangleElement.h"



class LobbySeatsPanel : public Panel
{
public:
	LobbySeatsPanel(sf::Vector2f size = { 160.0f, 90.0f }, sf::Vector2f position = { 0.0f, 0.0f }) : Panel(size, position), m_currentPlayersPerGame(Constants::Lobby::MinPlayersPerGame) {}

	void init(const UIElement& table, const sf::Vector2f& seatPositionGlobalSize, uint32_t currentPlayersPerGame)
	{
		assert(currentPlayersPerGame >= Constants::Lobby::MinPlayersPerGame && currentPlayersPerGame <= Constants::Lobby::MaxPlayersPerGame);

		setChildrenHitTest(true);
		setTransparentToInput(true);

		m_currentPlayersPerGame = currentPlayersPerGame;
		sf::Vector2f seatPositionLocalSize = { seatPositionGlobalSize.x / getGlobalScale().x, seatPositionGlobalSize.y / getGlobalScale().y };

		sf::Vector2f tableGlobalPosition = table.getGlobalTransform().transformPoint({ 0.0f, 0.0f });
		sf::Vector2f tableGlobalSize = table.getGlobalSize();

		sf::Vector2f downGlobalPosition = { tableGlobalPosition.x + tableGlobalSize.x * 0.5f, tableGlobalPosition.y + tableGlobalSize.y };
		sf::Vector2f leftGlobalPosition = { tableGlobalPosition.x, tableGlobalPosition.y + tableGlobalSize.y * 0.5f };
		sf::Vector2f topGlobalPosition = { tableGlobalPosition.x + tableGlobalSize.x * 0.5f, tableGlobalPosition.y };
		sf::Vector2f rightGlobalPosition = { tableGlobalPosition.x + tableGlobalSize.x, tableGlobalPosition.y + tableGlobalSize.y * 0.5f };

		auto inverse = getGlobalTransform().getInverse();

		m_seatsByLocation.emplace("down", &static_cast<RoundedRectangleElement&>(addChild(std::make_unique<RoundedRectangleElement>(seatPositionLocalSize, inverse.transformPoint(downGlobalPosition)))));
		m_seatsByLocation.emplace("left", &static_cast<RoundedRectangleElement&>(addChild(std::make_unique<RoundedRectangleElement>(seatPositionLocalSize, inverse.transformPoint(leftGlobalPosition)))));
		m_seatsByLocation.emplace("top", &static_cast<RoundedRectangleElement&>(addChild(std::make_unique<RoundedRectangleElement>(seatPositionLocalSize, inverse.transformPoint(topGlobalPosition)))));
		m_seatsByLocation.emplace("right", &static_cast<RoundedRectangleElement&>(addChild(std::make_unique<RoundedRectangleElement>(seatPositionLocalSize, inverse.transformPoint(rightGlobalPosition)))));

		for (const auto& [location, seat] : m_seatsByLocation)
			m_locationBySeats.emplace(seat, location);

		for (auto& seat : m_seatsByLocation)
		{
			auto& s = *seat.second;
			s.setOrigin(s.getGeometricCenter());
			s.shape().setFillColor(sf::Color::Black);
		}

		updateSeatPositions(m_currentPlayersPerGame);
	}

	void updateSeatPositions(uint32_t currentPlayersPerGame)
	{
		if (currentPlayersPerGame == Constants::Lobby::MaxPlayersPerGame)
			for (auto& seat : m_seatsByLocation)
				seat.second->setVisible(true);
		else
			for (auto& seat : m_seatsByLocation)
				seat.second->setVisible(false);

		switch (currentPlayersPerGame)
		{
			case 2:
				m_seatsByLocation.find("down")->second->setVisible(true);
				m_seatsByLocation.find("top")->second->setVisible(true);

				break;

			case 3:
				m_seatsByLocation.find("down")->second->setVisible(true);
				m_seatsByLocation.find("left")->second->setVisible(true);
				m_seatsByLocation.find("right")->second->setVisible(true);

				break;
		}
	}

	sf::Vector2f getGlobalSeatPositionBySeatIndex(uint32_t index) const
	{
		assert(index < Constants::Lobby::MaxPlayersPerGame && index >= 0);
		assert(index < m_currentPlayersPerGame);
		assert(m_currentPlayersPerGame <= Constants::Lobby::MaxPlayersPerGame && m_currentPlayersPerGame >= Constants::Lobby::MinPlayersPerGame);

		switch (index)
		{
			case 0:
				return m_seatsByLocation.find("down")->second->getGlobalGeometricCenter();
				break;

			case 1:
				if (m_currentPlayersPerGame == 2)
					return m_seatsByLocation.find("top")->second->getGlobalGeometricCenter();
				else
					return m_seatsByLocation.find("left")->second->getGlobalGeometricCenter();

				break;

			case 2:
				if (m_currentPlayersPerGame == 3)
					return m_seatsByLocation.find("right")->second->getGlobalGeometricCenter();
				else
					return m_seatsByLocation.find("top")->second->getGlobalGeometricCenter();

				break;

			case 3:
				return m_seatsByLocation.find("right")->second->getGlobalGeometricCenter();
				break;
		}
	}

	sf::Vector2f getGlobalSeatPositionBySeatElement(UIElement& seat) const
	{
		return getGlobalSeatPositionBySeatIndex(getSeatIndexBySeatElement(seat));
	}


	uint32_t getSeatIndexBySeatElement(UIElement& seat) const
	{
		bool found = false;
		for (const auto& child : m_children)
		{
			if (child.get() == &seat)
			{
				found = true;
				break;
			}
		}

		assert(found);
		assert(seat.isVisible());

		auto it = m_locationBySeats.find(&static_cast<RoundedRectangleElement&>(seat));
		const std::string& location = it->second;

		switch (m_currentPlayersPerGame)
		{
			case (2):
				if (location == "down")
					return 0;
				else if (location == "top")
					return 1;

				break;

			case (3):
				if (location == "down")
					return 0;
				else if (location == "left")
					return 1;
				else if (location == "right")
					return 2;

				break;

			case (4):
				if (location == "down")
					return 0;
				else if (location == "left")
					return 1;
				else if (location == "top")
					return 2;
				else if (location == "right")
					return 3;

				break;
		}

		assert(false);
	}

private:
	std::unordered_map<std::string, RoundedRectangleElement*> m_seatsByLocation;
	std::unordered_map<RoundedRectangleElement*, std::string> m_locationBySeats;

	uint32_t m_currentPlayersPerGame;
};