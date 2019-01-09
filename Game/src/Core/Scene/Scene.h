#pragma once
#include "Core/Entity/Entity.h"

class SystemBase;
class CollisionScene;

class Scene
{
public:
	Scene();
	~Scene();
	void Tick();

	Array<Entity*> entity_list;
	Entity* CreateEntity(const char* name);
	void DestroyEntity(Entity* entity);

	template<typename TComp>
	TComp* GetStaticComponent();

	CollisionScene* GetCollisionScene() const { return collision_scene; }

private:
	Array<SystemBase*> system_list;
	template<typename TSystem>
	void AddSystem();

	Entity* static_entity;
	CollisionScene* collision_scene;
};

template<typename TComp>
TComp* Scene::GetStaticComponent()
{
	if (static_entity == nullptr)
		static_entity = CreateEntity("[Static]");
	return static_entity->GetOrAddComponent<TComp>();
}

template<typename TSystem>
void Scene::AddSystem()
{
	TSystem* system = new TSystem();

	system->SetOwningScene(this);
	system->Setup();
	system_list.Add(system);
}

extern Scene* gScene;
