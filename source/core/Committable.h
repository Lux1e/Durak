#pragma once
#include <vector>
#include <memory>



struct IAction
{
	virtual void call() = 0;
	virtual ~IAction() = default;
};


template<typename Callable>
	requires std::invocable<Callable>
class BaseAction : public IAction
{
public:
	template <typename U>
	BaseAction(U&& u) : m_t(std::forward<U>(u)) {}

	void call() override { m_t(); }

private:
	std::decay_t<Callable> m_t;
};


class Committable
{
public:
	void commit()
	{
		commitChildren();
		commitDeferredActions();
	}

protected:
	template <typename T>
	void defer(T&& t)
	{
		using type = std::decay_t<T>;
		deferredActions.emplace_back(std::make_unique<BaseAction<type>>(std::forward<T>(t)));
	}

	virtual void commitChildren() {}

	void commitDeferredActions()
	{
		for (auto& action : deferredActions)
		{
			action->call();
		}

		deferredActions.clear();
	}

private:
	std::vector<std::unique_ptr<IAction>> deferredActions;
};