#pragma once
#include <cstdint>



struct ControlState
{
	struct Interaction
	{
		bool hovered;
		bool pressed;
		bool dragged;
		bool disabled = false;
	} interaction;

	struct Locks
	{
		uint32_t hover = 0;
		uint32_t press = 0;
		uint32_t click = 0;
		uint32_t drag = 1;
	} locks;
};