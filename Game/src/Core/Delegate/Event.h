#pragma once
#include "Delegate.h"

template<typename... InArgs>
class Event
{
	typedef Delegate<void, InArgs...> EventDelegate;

public:
	Event() {}

	void Add(const EventDelegate& del)
	{
		callList.Add(del);
	}
	void Add(EventDelegate&& del)
	{
		callList.Add(std::move(del));
	}

	void AddRaw(void(*func)(InArgs...))
	{
		Add(EventDelegate::Raw(func));
	}
	template<typename InObject>
	void AddObject(InObject* object, void (InObject::*func)(InArgs...))
	{
		Add(EventDelegate::Object(object, func));
	}
	template<typename InFunctor>
	void AddLambda(const InFunctor& functor)
	{
		Add(EventDelegate::Lambda(functor));
	}

	void Broadcast(InArgs... args)
	{
		for (uint32 i = 0; i < callList.Size(); ++i)
		{
			callList[i](args...);
		}
	}

	void Clear()
	{
		callList.Clear();
	}

private:
	Array<EventDelegate> callList;
};