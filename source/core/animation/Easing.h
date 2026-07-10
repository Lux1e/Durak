#pragma once



enum class EaseType
{
	Linear,

	InCubic,
	OutCubic,
	InOutCubic
};


float applyEase(EaseType type, float progress);