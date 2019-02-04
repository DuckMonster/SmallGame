#pragma once
#include "Collision.h"
#include "Core/Component/Component.h"

class TransformComponent;

class ColliderComponent : public Component
{
public:
	typedef Event<const OverlapResult&> OnOverlapEvent;

	void Setup() override;
	void Destroy() override;

	ColliderObject* object;
	OnOverlapEvent on_overlap;
	bool debug_draw = true;

private:
	// We want to poll if the transform is changed, because then we need
	//		to update our collision transforms
	void HandleTransformInvalidated(TransformComponent* transform);

	// Called when our collider-object overlaps with some other collider
	void HandleOverlap(const OverlapResult& result);
};

