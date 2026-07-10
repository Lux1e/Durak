#pragma once
#include <random>



class Random
{
public:
	static std::mt19937_64& get()
	{
		static std::mt19937_64 rng(std::random_device{}());
		return rng;
	}
};