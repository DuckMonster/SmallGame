#include "ColliderComponent.h"
#include "Core/Entity/Entity.h"
#include "Core/Scene/Scene.h"
#include "Engine/Transform/TransformComponent.h"
#include "Engine/Debug/Debug.h"

REGISTER_TYPE(ColliderComponent);

void ColliderComponent::Setup()
{
	// Create the collider object in the collision scene
	auto collision = GetOwner()->GetScene()->GetCollisionScene();
	object = collision->CreateObject();
	object->owner = GetOwner();
	object->on_overlap.AddObject(this, &ColliderComponent::HandleOverlap);

	// Bind event on the transform component to check when it's changed
	auto transform = GetOwner()->GetComponent<TransformComponent>();
	if (transform == nullptr)
	{
		Debug::Print("Collider added without a transform attached [entity: '%s']", *GetOwner()->GetName());
	}
	else
	{
		transform->on_invalidated.AddObject(
			this,
			&ColliderComponent::HandleTransformInvalidated
		);

		// Start by invalidating
		HandleTransformInvalidated(transform);
	}
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

SphereCollider* ColliderComponent::AddSphere(const Vec3& origin, float radius)
{
	SphereCollider& sphere = object->spheres.AddRef();
	sphere.origin = origin;
	sphere.radius = radius;

	return &sphere;
}

BoxCollider* ColliderComponent::AddBox(const Vec3& position, const Vec3& size, const Quat& rotation/* = Quat::identity*/)
{
	Assert(size.x >= 0.f && size.y >= 0.f && size.z >= 0.f);

	BoxCollider& box = object->boxes.AddRef();
	box.Set(position, size, rotation);

	return &box;
}

// We want to poll if the transform is changed, because then we need
//		to update our collision transforms
void ColliderComponent::HandleTransformInvalidated(TransformComponent* transform)
{
	transform->GetMatrix(object->transform);
}

// Called when our collider-object overlaps with some other collider
void ColliderComponent::HandleOverlap(ColliderObject* other)
{
	on_overlap.Broadcast(other->owner);
}