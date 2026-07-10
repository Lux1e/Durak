#pragma once
#include <SFML/Graphics.hpp>
#include "TimedAnimation.h"
#include "../../utils/LerpUtils.h"



class RotateAnimation : public TimedAnimation
{
public:
	RotateAnimation(std::function<void(sf::Angle)> setRotation, EaseType easeType, const sf::Angle startAngle, const sf::Angle endAngle, float duration) : TimedAnimation(easeType, duration),
		m_setRotation(std::move(setRotation)), m_startAngle(startAngle), m_endAngle(endAngle) {}

protected:
	void apply(float easedProgress) override
	{
		m_setRotation(LerpUtils::lerp(m_startAngle, m_endAngle, easedProgress));
	}

private:
	std::function<void(sf::Angle)> m_setRotation;
	const sf::Angle m_startAngle;
	const sf::Angle m_endAngle;
};