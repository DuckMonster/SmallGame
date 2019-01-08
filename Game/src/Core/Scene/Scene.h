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

	Array<Entity*> entityList;
	Entity* CreateEntity(const char* name);
	void DestroyEntity(Entity* entity);

	template<typename TComp>
	TComp* GetStaticComponent();

	CollisionScene* GetCollisionScene() const { return collisionScene; }

private:
	Array<SystemBase*> systemList;
	template<typename TSystem>
	void AddSystem();

	Entity* staticEntity;
	CollisionScene* collisionScene;
};

template<typename TComp>
TComp* Scene::GetStaticComponent()
{
	if (staticEntity == nullptr)
		staticEntity = CreateEntity("[Static]");
	return staticEntity->GetOrAddComponent<TComp>();
}

template<typename TSystem>
void Scene::AddSystem()
{
	TSystem* system = new TSystem();

	system->SetOwningScene(this);
	system->Setup();
	systemList.Add(system);
}

extern Scene* gScene;
