#pragma once
#include "Core/Component/Component.h"
#include "Engine/Collision/Collision.h"

class MovementComponent : public Component
{
public:
	enum class Mode
	{
		Delta,
		Set,
		Teleport
	};

	// Adds translation to this frame movement
	void Translate(const Vec3& translation);
	// Sets the position that the movement system will translate to
	void SetPosition(const Vec3& new_position);

	// Adds rotation to this frame movement
	void Rotate(const Quat& delta);
	// Sets the rotation that the movement system will rotate to
	void SetRotation(const Quat& new_rotation);

	// Resets the component for this frame
	void ResetFrame();

	Mode position_mode;
	Vec3 position;

	Mode rotation_mode;
	Quat rotation;

	Array<OverlapResult> hits;
};