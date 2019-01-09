#include "Projectile.h"
#include "Core/Scene/Scene.h"
#include "Core/Class/TypeHelpers.h"
#include "Core/Resource/ResourceManager.h"
#include "Core/Resource/MeshResource.h"
#include "Engine/Capability/CapabilityStatics.h"
#include "Engine/Transform/TransformComponent.h"
#include "Engine/Rendering/RenderableComponent.h"
#include "Engine/Collision/ColliderComponent.h"
#include "Core/Resource/MaterialResource.h"

REGISTER_TYPE(ProjectileComponent);

// CAPABILITY
void ProjectileCapability::Setup()
{
	transform = GetOwner()->GetOrAddComponent<TransformComponent>();
	projectile = GetOwner()->GetOrAddComponent<ProjectileComponent>();

	auto collider = GetOwner()->GetComponent<ColliderComponent>();
	if (collider != nullptr)
	{
		collider->on_overlap.AddObject(this, &ProjectileCapability::HandleOverlap);
	}
}

void ProjectileCapability::Tick()
{
	// Update position
	transform->Translate(projectile->direction * 10.f * Time::Delta());
	time -= Time::Delta();
	if (time <= 0.f)
	{
		GetOwner()->GetScene()->DestroyEntity(GetOwner());
	}
}

void ProjectileCapability::HandleOverlap(Entity* other)
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
	p.renderable->mesh = &gResourceManager->Load<MeshResource>("Mesh/sphere.fbx")->mesh;
	p.renderable->material = &gResourceManager->Load<MaterialResource>("Material/default.json")->material;

	p.collider = p.entity->AddComponent<ColliderComponent>();
	p.collider->AddSphere(Vec3(0.f), 1.f);

	AddCapability<ProjectileCapability>(p.entity);

	return p;
}
