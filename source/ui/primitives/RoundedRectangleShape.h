#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Shape.hpp>
#include <vector>



class RoundedRectangleShape final : public sf::Shape
{
public:
	RoundedRectangleShape(sf::Vector2f size = { 0.0f, 0.0f }) : m_size(size)
	{
		init();
	}

	virtual std::size_t getPointCount() const override;
	virtual sf::Vector2f getPoint(std::size_t index) const override;

	void setSize(sf::Vector2f size);
	sf::Vector2f getSize() const;

	void setCornerRatio(float cornerRatio = 0.10f);
	float getCornerRatio() const;

private:
	std::vector<sf::Vector2f> points;

	sf::Vector2f m_size;

	float m_cornerRatio;
	float m_cornerFactor;
	size_t m_minPointsPerCorner;
	sf::Angle m_angle;


	void init();
	void updatePoints();
};