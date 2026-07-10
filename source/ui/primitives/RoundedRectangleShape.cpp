#include "RoundedRectangleShape.h"
#include <algorithm>



void RoundedRectangleShape::init()
{
	m_cornerRatio = 0.10f;
	m_cornerFactor = 3.0f;
	m_minPointsPerCorner = 8;
	m_angle = sf::degrees(90.0f);

	updatePoints();
	update();
}


void RoundedRectangleShape::updatePoints()
{
	float radius = std::min(m_size.x, m_size.y) * m_cornerRatio;
	std::size_t pointsPerCorner = std::max(m_minPointsPerCorner, static_cast<size_t>(radius * m_cornerFactor));
	std::size_t totalPoints = 4 * pointsPerCorner;

	sf::Angle step = sf::degrees(m_angle.asDegrees() / pointsPerCorner);


	sf::Vector2f centerPosition[4];
	centerPosition[0] = { 0.0f + radius, 0.0f + radius };
	centerPosition[1] = { m_size.x - radius, 0.0f + radius };
	centerPosition[2] = { m_size.x - radius, m_size.y - radius };
	centerPosition[3] = { 0.0f + radius, m_size.y - radius };

	points.clear();
	points.reserve(totalPoints);

	sf::Angle startAngles[4] = { sf::degrees(180.0f), sf::degrees(270.0f), sf::degrees(0.0f), sf::degrees(90.0f) };

	for (size_t i = 0; i < 4; ++i)
	{
		for (size_t j = 0; j < pointsPerCorner; ++j)
		{
			points.emplace_back(
				centerPosition[i].x + std::cos(startAngles[i].asRadians() + step.asRadians() * j) * radius,
				centerPosition[i].y + std::sin(startAngles[i].asRadians() + step.asRadians() * j) * radius);
		}
	}
}


std::size_t RoundedRectangleShape::getPointCount() const
{
	return points.size();
}

sf::Vector2f RoundedRectangleShape::getPoint(std::size_t index) const
{
	return points[index];
}


void RoundedRectangleShape::setSize(sf::Vector2f size)
{
	assert(size.x >= 0);
	assert(size.x >= 0);

	if (m_size == size)
		return;

	m_size = size;
	updatePoints();
	update();
}

sf::Vector2f RoundedRectangleShape::getSize() const
{
	return m_size;
}

void RoundedRectangleShape::setCornerRatio(float cornerRatio)
{
	m_cornerRatio = cornerRatio;
	updatePoints();
	update();
}

float RoundedRectangleShape::getCornerRatio() const
{
	return m_cornerRatio;
}