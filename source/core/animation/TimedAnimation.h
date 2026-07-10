#pragma once
#include "Animation.h"
#include "Easing.h"



class TimedAnimation : public Animation
{
public:
	TimedAnimation(EaseType easeType, float duration) : m_easeType(easeType), m_duration(duration)
	{
		assert(duration > 0.0f);
	}


	bool update(float dt) override
	{
		m_elapsedTime += dt;

		float progress = std::min(m_elapsedTime / m_duration, 1.0f);
		float easedProgress = applyEase(m_easeType, progress);

		apply(easedProgress);

		if (progress >= 1.0f)
		{
			finish();
			return true;
		}

		return false;
	}

	virtual void apply(float easedProgress) = 0;

private:
	const EaseType m_easeType;
	float m_duration;
	float m_elapsedTime{};
};