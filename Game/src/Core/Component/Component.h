#pragma once
#include "Core/Class/TypedObject.h"
#include "Core/Class/TypeHelpers.h"

class Entity;

template<typename TComp, typename TOther>
TComp* Cast(TOther* other)
{
	return dynamic_cast<TComp*>(other);
}

class Component : public TypedObject
{
public:
	virtual ~Component() {}

	// Called when adding a component to an entity
	virtual void Setup() {}

	// Called when being removed from and entity, for example when it is destroyed
	virtual void Destroy() {}

	template<typename TComp>
	bool IsA()
	{
		return Cast<TComp>(this) != nullptr;
	}

	template<typename TComp>
	TComp As()
	{
		return Cast<TComp>(this);
	}

	Entity* GetOwner() { return owner; }
	const Entity* GetOwner() const { return owner; }
	void SetOwner(Entity* new_owner)
	{
		Assert(owner == nullptr);
		owner = new_owner;
	}

private:
	Entity* owner = nullptr;
};
