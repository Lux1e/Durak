#pragma once
#include <memory>


struct AliveState { bool isAlive = true; };

class Trackable
{
public:
	Trackable() : lifeState(std::make_shared<AliveState>()) {}
	virtual ~Trackable() { lifeState->isAlive = false; }

	std::weak_ptr<AliveState> getState() const { return lifeState; }

private:
	std::shared_ptr<AliveState> lifeState;
};