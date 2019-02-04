#include "MovementSystem.h"
#include "Engine/Collision/ColliderComponent.h"
#include "Engine/Debug/Debug.h"

void MovementSystem::ProcessEntity(Entity* entity, TransformComponent* transform, MovementComponent* movement)
{
	Vec3 delta_position;
	Quat delta_rotation;

	// Get the new position
	if (movement->position_mode == MovementComponent::Mode::Delta)
	{
		transform->Translate(movement->position);
		delta_position = movement->position;
	}
	else
	{
		transform->SetPosition(movement->position);
		delta_position = movement->position - transform->GetWorldPosition();
	}

	// Get the new rotation
	if (movement->rotation_mode == MovementComponent::Mode::Delta)
	{
		transform->Rotate(movement->rotation);
		delta_rotation = movement->rotation;
	}
	else
	{
		transform->SetRotation(movement->rotation);
		delta_rotation = Quat::FromToQuat(transform->GetWorldRotation(), movement->rotation);
	}

	// If there is a collider on this entity, do collision checking when moving
	ColliderComponent* collider = entity->GetComponent<ColliderComponent>();
	if (collider != nullptr)
	{
		Mat4 delta_mat = Mat4::TranslateRotate(delta_position, delta_rotation);

		TArray<OverlapResult> hits;
		GetScene()->GetCollisionScene()->QueryDeltaMovement(
			collider->object,
			delta_mat,
			hits
		);

		movement->hits = hits;
		for(const OverlapResult& result : movement->hits)
		{
			transform->Translate(result.normal * result.penetration_depth);
		}
	}

	transform->Invalidate();
	movement->ResetFrame();
}