#pragma once
#include "Core/Component/Component.h"
#include "Core/Class/Type.h"

class Scene;

class Entity
{
public:
	Entity(const char* name, uint32 id, Scene* scene) : name(name), id(id), scene(scene) {}
	~Entity();

	template<typename TComp>
	TComp* GetComponent();

	template<typename TComp>
	TComp* AddComponent();

	Component* AddComponent(Type* type);

	template<typename TComp>
	TComp* GetOrAddComponent();

	const Array<Component*>& GetAllComponents() const { return components; }

	void SetName(const char* in_name) { name = in_name; }
	const String& GetName() const { return name; }
	uint32 GetId() const { return id; }
	Scene* GetScene() { return scene; }
	const Scene* GetScene() const { return scene; }

private:
	String name;
	Scene* const scene;
	const uint32 id;
	Array<Component*> components;
};

template<typename TComp>
TComp* Entity::GetComponent()
{
	for(uint32 i = 0; i<components.Size(); ++i)
	{
		TComp* casted_comp = Cast<TComp>(components[i]);
		if (casted_comp != nullptr)
			return casted_comp;
	}

	return nullptr;
}

template<typename TComp>
TComp* Entity::AddComponent()
{
	return (TComp*)AddComponent(Type::Get<TComp>());
}

template<typename TComp>
TComp* Entity::GetOrAddComponent()
{
	TComp* comp = GetComponent<TComp>();
	if (comp != nullptr)
		return comp;
	else
		return AddComponent<TComp>();
}
