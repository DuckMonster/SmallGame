
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
#include "Runtime/Movement/MovementComponent.h"
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
	p.renderable = p.entity->AddComponent<RenderableComponent>();
	p.movement = p.entity->AddComponent<MovementComponent>();

	// Create colliders
	p.collider->object->AddBox(Vec3(0.f, 1.f, 0.f), Vec3(1.f, 2.f, 1.f), Quat::identity);
	p.collider->debug_draw = false;

	// Load renderable
	p.renderable->AddRenderableLoad("Mesh/character.fbx", "Material/player.json");

	// Add capabilities
	AddCapability<PlayerMovementCapability>(p.entity);
	AddCapability<MouseAimCapability>(p.entity);
	AddCapability<AimLookCapability>(p.entity);
	AddCapability<PlayerFireCapability>(p.entity);
	return p;
}
