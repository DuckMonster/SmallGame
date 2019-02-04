#include "Projectile.h"
#include "Core/Scene/Scene.h"
#include "Core/Class/TypeHelpers.h"
#include "Core/Resource/ResourceManager.h"
#include "Core/Resource/MeshResource.h"
#include "Engine/Capability/CapabilityStatics.h"
#include "Engine/Transform/TransformComponent.h"
#include "Engine/Rendering/RenderableComponent.h"
#include "Engine/Collision/ColliderComponent.h"
#include "Engine/Debug/Debug.h"
#include "Runtime/Movement/MovementComponent.h"
#include "Core/Resource/MaterialResource.h"

REGISTER_TYPE(ProjectileComponent);

// CAPABILITY
void ProjectileCapability::Setup()
{
	projectile = GetOwner()->GetOrAddComponent<ProjectileComponent>();
	movement = GetOwner()->GetOrAddComponent<MovementComponent>();

	velocity = projectile->direction * 20.f;
}

void ProjectileCapability::Tick()
{
	// Handle collision hits
	for(const OverlapResult& hit : movement->hits)
	{
		//Debug::Print("Hit %s", *hit.other->owner->GetName(), VAL);
		if (Vec::Dot(velocity, hit.normal) > 0.f)
			continue;

		velocity = Vec::Reflect(velocity, hit.normal) * 0.8f;
	}

	// Gravity
	velocity += Vec3::up * -5.f * Time::Delta();

	// Update position
	movement->Translate(velocity * Time::Delta());
	time -= Time::Delta();
	if (time <= 0.f)
	{
		GetOwner()->GetScene()->DestroyEntity(GetOwner());
	}
}

void ProjectileCapability::HandleOverlap(const OverlapResult& result)
{
	// Kill ourselves on any overlap
	GetOwner()->GetScene()->DestroyEntity(GetOwner());
}

// PREFAB
ProjectilePrefab ProjectilePrefab::Create(Scene* scene, const Vec3& origin, const Vec3& direction)
{
	ProjectilePrefab p;

	p.entity = scene->CreateEntity("Projectile");
	p.transform = p.entity->AddComponent<TransformComponent>();
	p.transform->SetPosition(origin + Vec::Normalize(direction) * 1.f);
	p.transform->SetScale(Vec3(0.2f));

	p.projectile = p.entity->AddComponent<ProjectileComponent>();
	p.projectile->direction = Vec::Normalize(direction);

	p.renderable = p.entity->AddComponent<RenderableComponent>();
	p.renderable->AddRenderableLoad("Mesh/sphere.fbx", "Material/default.json");

	p.collider = p.entity->AddComponent<ColliderComponent>();
	p.collider->object->AddSphere(Vec3(0.f), 1.f);

	AddCapability<ProjectileCapability>(p.entity);

	return p;
}
