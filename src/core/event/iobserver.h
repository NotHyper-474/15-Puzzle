#pragma once

#include <functional>
#include <vector>

struct IPublisher
{
	using EventHandler = std::function<void()>;

	virtual ~IPublisher() = default;
	virtual void Attach(EventHandler hook) { _hooks.push_back(hook); }
	virtual void Detach(EventHandler hook) {
		auto it = std::remove_if(std::begin(_hooks), std::end(_hooks), [&](EventHandler& h) { return h.target<EventHandler>() == hook.target<EventHandler>(); });
		_hooks.erase(it, std::end(_hooks));
	}
	virtual void Notify()
	{
		auto iterator = _hooks.begin();
		while (iterator != _hooks.end())
		{
			(*iterator++)();
		}
	}

protected:
	std::vector<EventHandler> _hooks;
};