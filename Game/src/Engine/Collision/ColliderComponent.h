#pragma once
#include "Collision.h"
#include "Core/Component/Component.h"

class TransformComponent;

class ColliderComponent : public Component
{
public:
	typedef Event<Entity*> OnOverlapEvent;

	void Setup() override;
	void Destroy() override;

	SphereCollider* AddSphere() { return AddSphere(Vec3(0.f), 1.f); }
	SphereCollider* AddSphere(const Vec3& origin, float radius);

	BoxCollider* AddBox() { return AddBox(Vec3(0.f), Vec3(1.f)); }
	BoxCollider* AddBox(const Vec3& position, const Vec3& size, const Quat& rotation = Quat::identity);

	OnOverlapEvent on_overlap;

private:
	// We want to poll if the transform is changed, because then we need
	//		to update our collision transforms
	void HandleTransformInvalidated(TransformComponent* transform);

	// Called when our collider-object overlaps with some other collider
	void HandleOverlap(ColliderObject* other);
	ColliderObject* object;
};

