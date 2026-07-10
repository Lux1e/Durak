#pragma once
#include <vector>



struct PendingPacket
{
	std::vector<uint8_t> data;
	std::size_t sent = 0;
};