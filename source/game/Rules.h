#pragma once
#include"../core/Trackable.h"


struct Rules
{
	virtual ~Rules() = default;

	virtual void update(float dt) = 0;
};