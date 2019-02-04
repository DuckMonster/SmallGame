#include "MovementComponent.h"

REGISTER_TYPE(MovementComponent);

// Adds translation to this frame movement
void MovementComponent::Translate(const Vec3& translation)
{
	position += translation;
}

// Sets the position that the movement system will translate to
void MovementComponent::SetPosition(const Vec3& new_position)
{
	position = new_position;
	position_mode = Mode::Set;
}

// Adds rotation to this frame movement
void MovementComponent::Rotate(const Quat& delta)
{
	rotation = Quat::Normalize(delta * rotation);
}

// Sets the rotation that the movement system will rotate to
void MovementComponent::SetRotation(const Quat& new_rotation)
{
	rotation = new_rotation;
	rotation_mode = Mode::Set;
}

// Resets the component for this frame
void MovementComponent::ResetFrame()
{
	position_mode = Mode::Delta;
	position = Vec3::zero;

	rotation_mode = Mode::Delta;
	rotation = Quat::identity;
}