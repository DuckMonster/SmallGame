
#include "Player.h"
#include "Core/Class/TypeHelpers.h"
#include "Core/Scene/Scene.h"
#include "Core/Resource/MeshResource.h"
#include "Core/Resource/MaterialResource.h"
#include "Core/Resource/ResourceManager.h"
#include "Engine/Rendering/RenderableComponent.h"
#include "Engine/Transform/TransformComponent.h"
#include "Engine/Capability/CapabilityStatics.h"
#include "Engine/Collision/ColliderComponent.h"
#include "Engine/Camera/CameraComponent.h"
#include "Movement/PlayerMovementCapability.h"
#include "Movement/MouseAimCapability.h"
#include "Movement/AimLookCapability.h"
#include "Combat/PlayerFireCapability.h"

REGISTER_TYPE(PlayerComponent);
REGISTER_PROP(PlayerComponent, aim_world_position);
REGISTER_PROP(PlayerComponent, follower_camera);

PlayerPrefab PlayerPrefab::Create(Scene* scene)
{
	/* Create player entity */
	PlayerPrefab p;
	p.entity = scene->CreateEntity("Player");
	p.component = p.entity->AddComponent<PlayerComponent>();
	p.transform = p.entity->AddComponent<TransformComponent>();
	p.collider = p.entity->AddComponent<ColliderComponent>();

	// Create colliders
	p.collider->AddBox(Vec3(0.f, 1.f, 0.f), Vec3(1.f, 2.f, 1.f));

	/* Create mesh entity */
	p.mesh_entity = scene->CreateEntity("Player Mesh");
	p.mesh_transform = p.mesh_entity->AddComponent<TransformComponent>();
	p.mesh_transform->SetParent(p.transform);
	p.mesh_renderable = p.mesh_entity->AddComponent<RenderableComponent>();

	// Load renderable
	MeshResource* mesh			= gResourceManager->Load<MeshResource>("Mesh/character.fbx");
	MaterialResource* material	= gResourceManager->Load<MaterialResource>("Material/default.json");

	p.mesh_renderable->mesh = &mesh->mesh;
	p.mesh_renderable->material = &material->material;

	p.component->mesh_entity = p.mesh_entity;

	// Add capabilities
	AddCapability<PlayerMovementCapability>(p.entity);
	AddCapability<MouseAimCapability>(p.entity);
	AddCapability<AimLookCapability>(p.entity);
	AddCapability<PlayerFireCapability>(p.entity);
	return p;
}
