#pragma once
#include <SFML/Graphics.hpp>
#include "TimedAnimation.h"
#include "../../utils/LerpUtils.h"



class MoveAnimation : public TimedAnimation
{
public:
	MoveAnimation(std::function<void(sf::Vector2f)> setPosition, EaseType easeType, const sf::Vector2f& startPosition, const sf::Vector2f& endPosition, float duration) :
		TimedAnimation(easeType, duration), m_setPosition(std::move(setPosition)), m_startPosition(startPosition), m_endPosition(endPosition) {}

protected:
	void apply(float easedProgress) override
	{
		m_setPosition(LerpUtils::lerp(m_startPosition, m_endPosition, easedProgress));
	}

private:
	std::function<void(sf::Vector2f)> m_setPosition;
	const sf::Vector2f m_startPosition;
	const sf::Vector2f m_endPosition;
};