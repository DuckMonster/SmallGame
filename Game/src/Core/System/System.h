#pragma once
#include <vector>
#include "Core/Entity/Entity.h"
#include "Core/Scene/Scene.h"

class Entity;

class SystemBase
{
public:
	virtual ~SystemBase() {}

	virtual void Setup() {}
	void SetOwningScene(Scene* scene) { this->scene = scene; }
	virtual void RunSystem() = 0;

protected:
	Scene* GetScene() { return scene; }

private:
	Scene* scene;
};

// Standard system which simply runs, nothing special
class System : public SystemBase
{
public:
	void RunSystem() override
	{
		Run();
	}

protected:
	virtual void Run() = 0;
};

// System which iterates over entities who contain a subset of components
template<typename... TComponents>
class ComponentSystem : public SystemBase
{
public:
	void RunSystem() override
	{
		Scene* scene = GetScene();

		RunBegin();
		for (uint32 i = 0; i < scene->entityList.Size(); ++i)
		{
			if (scene->entityList[i] == nullptr)
				continue;

			RunFor<TComponents...>(scene->entityList[i]);
		}
		RunEnd();
	}

protected:
	virtual void RunBegin() {}
	virtual void ProcessEntity(Entity* entity, TComponents*... components) = 0;
	virtual void RunEnd() {}

private:
	template<typename... TRead>
	bool RunFor(Entity* entity, TRead*... readComponents)
	{
		ProcessEntity(entity, readComponents...);
		return true;
	}
	template<typename TToRead, typename... TLeft, typename... TRead>
	bool RunFor(Entity* entity, TRead*... readComponents)
	{
		TToRead* comp = entity->GetComponent<TToRead>();
		if (comp == nullptr)
			return false;

		return RunFor<TLeft...>(entity, readComponents..., comp);
	}
};