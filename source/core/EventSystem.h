#pragma once
#include <vector>
#include <memory>
#include <cstring>
#include <stdexcept>
#include "Trackable.h"
#include "../game/events/Event.h"
#include "EventInitiator.h"



class EventSystem
{
public:
	EventSystem() = default;

	EventSystem(const EventSystem&) = delete;
	EventSystem& operator=(const EventSystem&) = delete;
	EventSystem(EventSystem&&) = delete;
	EventSystem& operator=(EventSystem&&) = delete;


	template <typename EventT, typename T>
	void subscribe(T* object, void(T::* method)(const EventT&, const EventInitiator&))
	{
		static_assert(std::is_base_of_v<Event, EventT>);
		static_assert(std::is_base_of_v<Trackable, T>);
		static_assert(std::is_trivially_copyable_v<decltype(method)>,
			"pointer-to-member must be trivially copyable for this storage method");

		Handler handler;
		handler.obj = static_cast<Trackable*>(object);

		handler.lifeState = object->getState();

		handler.methodStorage.resize(sizeof(method));
		std::memcpy(handler.methodStorage.data(), &method, sizeof(method));

		handler.caller = [](Trackable* obj, const Event* ev, const EventInitiator* initiatorPtr, const std::vector<uint8_t>& storage)
			{
				using MethodT = void(T::*)(const EventT&, const EventInitiator&);
				MethodT realMethod;
				std::memcpy(&realMethod, storage.data(), sizeof(realMethod));
				(static_cast<T*>(obj)->*realMethod)(*static_cast<const EventT*>(ev), *initiatorPtr);
			};

		getHandlers<EventT>().push_back(std::move(handler));
	}

	template <typename EventT>
	void publish(EventT event, std::optional<uint32_t> playerId = std::nullopt)
	{
		static_assert(std::is_base_of_v<Event, EventT>);

		QueueItem item;
		item.eventPtr = std::make_unique<EventT>(std::move(event));
		item.initiator = EventInitiator(std::move(playerId));
		item.dispatcher = [](EventSystem* sys, const Event* ev, const EventInitiator* initiator)
			{
				auto& handlers = sys->getHandlers<EventT>();

				for (auto& h : handlers)
				{
					if (auto st = h.lifeState.lock())
					{
						if (st->isAlive)
						{
							h.caller(h.obj, ev, initiator, h.methodStorage);
						}
					}
				}

				handlers.erase(std::remove_if(handlers.begin(), handlers.end(),
					[](Handler& h)
					{
						auto st = h.lifeState.lock();
						return (!st || !st->isAlive);
					}), handlers.end());
			};

		nextQueue.push_back(std::move(item));
	}

	void dispatch()
	{
		int MAX_EVENTS_PER_FRAME = 1000;
		int processedEvents = 0;

		do
		{
			for (auto& item : queue)
			{
				item.dispatcher(this, item.eventPtr.get(), &item.initiator);
				++processedEvents;

				if (processedEvents > MAX_EVENTS_PER_FRAME)
					throw std::runtime_error("Too many events in frame");
			}

			queue.clear();
			std::swap(queue, nextQueue);

		} while (!queue.empty());
	}

private:
	struct Handler
	{
		Trackable* obj;
		std::weak_ptr<AliveState> lifeState;
		std::vector<uint8_t> methodStorage;
		void (*caller)(Trackable*, const Event*, const EventInitiator*, const std::vector<uint8_t>&) = nullptr;
	};

	struct QueueItem
	{
		std::unique_ptr<Event> eventPtr;
		void (*dispatcher)(EventSystem*, const Event*, const EventInitiator*) = nullptr;
		EventInitiator initiator;
	};

	std::vector<QueueItem> queue;
	std::vector<QueueItem> nextQueue;

	template<typename EventT>
	std::vector<Handler>& getHandlers()
	{
		static std::vector<Handler> vec;
		return vec;
	}
};