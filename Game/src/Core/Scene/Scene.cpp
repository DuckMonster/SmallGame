#include "Scene.h"
#include <GL/glew.h>
#include "Engine/Rendering/RenderSystem.h"
#include "Engine/Capability/CapabilitySystem.h"
#include "Core/Resource/ResourceManager.h"
#include "Core/Resource/MeshResource.h"
#include "Core/Resource/MaterialResource.h"
#include "Core/Resource/TextureResource.h"
#include "Runtime/Player/Player.h"
#include "Runtime/Camera/FollowCamera.h"
#include "Engine/Collision/Collision.h"
#include "Engine/GUI/GuiSystem.h"
#include "Engine/Rendering/RenderStaticComponent.h"
#include "Engine/Collision/CollisionSystem.h"
#include "Engine/Debug/Debug.h"

Scene* gScene;

Scene::Scene()
{
	gScene = this;

	// Create collision
	collision_scene = new CollisionScene();

	AddSystem<CapabilitySystem>();
	AddSystem<CollisionSystem>();
	AddSystem<RenderSystem>();
	AddSystem<GuiSystem>();
	
	PlayerPrefab player = PlayerPrefab::Create(this);

	{
		// Temp floor
		auto entity = CreateEntity("Floor");
		auto renderable = entity->AddComponent<RenderableComponent>();
		auto transform = entity->AddComponent<TransformComponent>();
		transform->SetScale(50.f);

		renderable->mesh = &gResourceManager->Load<MeshResource>("Mesh/plane.fbx")->mesh;
		renderable->material = &gResourceManager->Load<MaterialResource>("Material/floor.json")->material;
	}
	{
		// Other test collider
		auto entity = CreateEntity("TestCollider");
		auto collider = entity->AddComponent<ColliderComponent>();
		collider->on_overlap.AddLambda([](Entity* other) { Debug::PrintOneFrame("Did I hit something?"); });
		collider->AddSphere(Vec3(7.f, 0.f, 0.f), 4.f);
		// collider->AddBox(
		// 	Vec3(-7.f, 2.f, 1.f),
		// 	Vec3(4.f, 2.5f, 2.f),
		// 	Quat::AngleAxis(PI / 4.f, Vec3::Up)
		// );
		collider->AddBox(Vec3(-7.f, 2.f, 2.f), Vec3(2.f, 2.f, 5.f));
	}
	{
		FollowCamera camera = FollowCamera::Create(this, player.entity);
		GetStaticComponent<RenderStaticComponent>()->active_camera = camera.camera;
	}
}

Scene::~Scene()
{
}

void Scene::Tick()
{
	for (SystemBase* system : system_list)
	{
		system->RunSystem();
	}
}

Entity* Scene::CreateEntity(const char* name)
{
	// Find an empty id
	uint32 id = -1;
	for (uint32 i = 0; i < entity_list.Size(); ++i)
	{
		if (entity_list[i] == nullptr)
		{
			id = i;
			break;
		}
	}

	// No empty slot found, just add a new element and set ID to that
	if (id == -1)
	{
		id = entity_list.Size();
		entity_list.AddRef();
	}

	Entity* entity = new Entity(name, id, this);
	entity_list[id] = entity;

	return entity;
}

void Scene::DestroyEntity(Entity* entity)
{
	Assert(entity != nullptr);

	entity_list[entity->GetId()] = nullptr;
	delete entity;
}
