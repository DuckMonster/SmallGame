#pragma once
#include "Core/Component/Component.h"

class TransformComponent : public Component
{
public:
	typedef Event<TransformComponent*> OnInvalidatedEvent;

	/* Position */
	// Gets the local-space position of this transform
	const Vec3& GetPosition() const { return transform.position; }
	// Gets the world-space position of this transform
	Vec3 GetWorldPosition() const;

	// Sets the position of this transform
	void SetPosition(const Vec3& in_position);
	// Translates this transform by some delta location
	void Translate(const Vec3& delta);

	/* Rotation */
	// Gets the local-space rotation
	const Quat& GetRotation() const { return transform.rotation; }
	// Gets the world-space rotation
	Quat GetWorldRotation() const;

	// Sets the rotation of this transform
	void SetRotation(const Quat& in_rotation);
	// Rotates this transform by some delta
	void Rotate(const Quat& delta);

	/* Scale */
	const Vec3& GetScale() const { return transform.scale; }

	// Sets the scale of this transform
	void SetScale(const Vec3& in_scale);

	// Gets the matrix representing this transform (out_matrix is assumed to be identity!)
	Mat4 GetMatrix();

	// Marks this transform as changed, so matrices and such will be updated
	void Invalidate();

	// Attaches this transform to another transform, making all values relative
	void SetParent(TransformComponent* in_parent);

	// Event for when this transform is invalidated, so that
	// 		dependent systems can update accordingly
	OnInvalidatedEvent on_invalidated;

	Transform transform = Transform::identity;

private:
	void HandleParentInvalidated(TransformComponent* parent) { Invalidate(); }

	Mat4 cached_matrix;
	bool is_dirty = true;

	TransformComponent* parent = nullptr;
};