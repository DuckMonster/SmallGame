#include "ColliderComponent.h"
#include "Core/Entity/Entity.h"
#include "Core/Scene/Scene.h"

REGISTER_TYPE(ColliderComponent);

void ColliderComponent::Setup()
{
	// Create the collider object in the collision scene
	auto collision = GetOwner()->GetScene()->GetCollisionScene();
	object = collision->CreateObject();
	object->owner = GetOwner();
	object->on_overlap.AddObject(this, &ColliderComponent::HandleOverlap);
}

void ColliderComponent::Destroy()
{
	if (object == nullptr)
	{
		Error("Destroy() called without a collider object");
		return;
	}

	auto collision = GetOwner()->GetScene()->GetCollisionScene();
	collision->DestroyObject(object);
}

// Called when our collider-object overlaps with some other collider
void ColliderComponent::HandleOverlap(const OverlapResult& result)
{
	on_overlap.Broadcast(result);
}