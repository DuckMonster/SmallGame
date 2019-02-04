#include "Scene.h"
#include <GL/glew.h>
#include "Engine/Rendering/RenderSystem.h"
#include "Engine/Capability/CapabilitySystem.h"
#include "Core/Resource/ResourceManager.h"
#include "Core/Resource/MeshResource.h"
#include "Core/Resource/MaterialResource.h"
#include "Core/Resource/TextureResource.h"
#include "Engine/Shape/Shape.h"
#include "Engine/Collision/Collision.h"
#include "Engine/GUI/GuiSystem.h"
#include "Engine/Rendering/RenderStaticComponent.h"
#include "Engine/Collision/CollisionSystem.h"
#include "Engine/Collision/CollisionDebugSystem.h"
#include "Engine/Debug/Debug.h"
#include "Engine/Light/DirectionalLightComponent.h"
#include "Runtime/Player/Player.h"
#include "Runtime/Camera/FollowCamera.h"
#include "Runtime/Movement/MovementSystem.h"

Scene* gScene;

Scene::Scene()
{
	gScene = this;

	// Create collision
	collision_scene = new CollisionScene();

	AddSystem<CapabilitySystem>();
	AddSystem<MovementSystem>();
	AddSystem<CollisionSystem>();
	AddSystem<CollisionDebugSystem>();
	AddSystem<RenderSystem>();
	AddSystem<GuiSystem>();
	
	PlayerPrefab player = PlayerPrefab::Create(this);

	{
		// Temp floor
		auto entity = CreateEntity("Floor");
		auto transform = entity->AddComponent<TransformComponent>();
		transform->SetScale(50.f);

		auto renderable = entity->AddComponent<RenderableComponent>();
		renderable->AddRenderableLoad("Mesh/plane.fbx", "Material/floor.json");

		auto collider = entity->AddComponent<ColliderComponent>();
		collider->debug_draw = false;
		collider->object->AddBox(
			Vec3(0.f, -0.5f, 0.f),
			Vec3(50.f, 1.f, 50.f),
			Quat::identity
		);
	}
	{
		// Other test collider
		ShapePrefab::CreateCube(
			this, true,
			Transform(
				Vec3(-4.f, 0.f, 3.f),
				Quat::identity,
				Vec3(2.f, 0.5f, 4.f)
			)
		);
		ShapePrefab::CreateCube(
			this, true,
			Transform(
				Vec3(-5.f, 2.f, -1.f),
				Quat::AngleAxis(Math::Radians(25.f), Vec::Normalize(Vec3(2.f, 4.f, -2.f))),
				Vec3(7.f, 2.f, 4.f)
			)
		);

		ShapePrefab::CreateSphere(
			this, true,
			Transform(
				Vec3(8.f, 0.f, 2.f),
				Quat::identity,
				Vec3(1.f)
			)
		);
	}
	{
		auto render_static = GetStaticComponent<RenderStaticComponent>();
		// Camera
		FollowCamera camera = FollowCamera::Create(this, player.entity);
		render_static->active_camera = camera.camera;

		// Light source
		Entity* entity = CreateEntity("Directional Light");
		auto light = entity->AddComponent<DirectionalLightComponent>();

		light->position = Vec3(5, 5, 5);
		light->direction = Vec::Normalize(-light->position);
		render_static->light = light;
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
