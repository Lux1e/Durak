#pragma once
#include <cstdint>
#include "../protocol/ClientRole.h"



struct ClientContext
{
	uint32_t localId;
	ClientRole localRole;
};