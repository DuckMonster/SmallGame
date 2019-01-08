#pragma once
#include "Core/Component/Component.h"

class TransformComponent : public Component
{
public:
	typedef Event<TransformComponent*> OnInvalidatedEvent;

	/* Position */
	// Gets the local-space position of this transform
	const Vec3& GetPosition() const { return position; }
	// Gets the world-space position of this transform
	Vec3 GetWorldPosition() const;

	// Sets the position of this transform
	void SetPosition(const Vec3& inPosition);
	// Translates this transform by some delta location
	void Translate(const Vec3& delta);

	/* Rotation */
	const Quat& GetRotation() const { return rotation; }

	// Sets the rotation of this transform
	void SetRotation(const Quat& inRotation);
	// Rotates this transform by some delta
	void Rotate(const Quat& delta);

	/* Scale */
	const Vec3& GetScale() const { return scale; }

	// Sets the scale of this transform
	void SetScale(const Vec3& inScale);

	// Gets the matrix representing this transform (outMatrix is assumed to be identity!)
	void GetMatrix(Mat4& outMatrix);

	// Marks this transform as changed, so matrices and such will be updated
	void Invalidate();

	// Attaches this transform to another transform, making all values relative
	void SetParent(TransformComponent* in_parent);

	// Event for when this transform is invalidated, so that
	// 		dependent systems can update accordingly
	OnInvalidatedEvent onInvalidated;

private:
	void HandleParentInvalidated(TransformComponent* parent) { Invalidate(); }

	Vec3 position = Vec3(0.f);
	Quat rotation = Quat(0.f, 0.f, 0.f, 1.f);
	Vec3 scale = Vec3(1.f);

	Mat4 cachedMatrix;
	bool isDirty = true;

	TransformComponent* parent = nullptr;
};