#include "Easing.h"
#include "../../utils/EasingUtils.h"



float applyEase(EaseType type, float progress)
{
	switch (type)
	{
		case EaseType::Linear:
			return Easing::linear(progress);

		case EaseType::InCubic:
			return Easing::easeInCubic(progress);

		case EaseType::OutCubic:
			return Easing::easeOutCubic(progress);

		case EaseType::InOutCubic:
			return Easing::easeInOutCubic(progress);
	}
}