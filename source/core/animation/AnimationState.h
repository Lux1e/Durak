#pragma once
#include <vector>
#include <memory>
#include "Animation.h"



struct AnimationState
{
	std::vector<std::unique_ptr<Animation>> animations;
};