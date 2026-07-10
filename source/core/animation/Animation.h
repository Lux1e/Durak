#pragma once
#include <functional>



class Animation
{
public:
	virtual ~Animation() = default;

	virtual bool update(float dt) = 0;

	void setOnFinish(std::function<void()> func)
	{
		onFinish = std::move(func);
	}

protected:
	void finish()
	{
		if (onFinish)
			onFinish();
	}

private:
	std::function<void()> onFinish;
};