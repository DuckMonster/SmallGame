
#include "Core/Class/TypeHelpers.h"
#include "TransformComponent.h"

REGISTER_TYPE(TransformComponent);
// REGISTER_PROP(TransformComponent, position);
// REGISTER_PROP(TransformComponent, rotation);
// REGISTER_PROP(TransformComponent, scale);

// Gets the world-space position of this transform
Vec3 TransformComponent::GetWorldPosition() const
{
	if (parent == nullptr)
		return position;

	Mat4 parent_mat;
	parent->GetMatrix(parent_mat);

	return parent_mat * position;
}

// Sets the position of this transform
void TransformComponent::SetPosition(const Vec3& inPosition)
{
	position = inPosition;
	Invalidate();
}
// Translates this transform by some delta location
void TransformComponent::Translate(const Vec3& delta)
{
	position += delta;
	Invalidate();
}

// Sets the rotation of this transform
void TransformComponent::SetRotation(const Quat& inRotation)
{
	rotation = inRotation;
	Invalidate();
}
// Rotates this transform by some delta
void TransformComponent::Rotate(const Quat& delta)
{
	// Since rotation multiplication are right-to-left (so right-most is applied first)
	//		we want to do our current rotation _first_, then the delta
	rotation = delta * rotation;
	Invalidate();
}

// Sets the scale of this transform
void TransformComponent::SetScale(const Vec3& inScale)
{
	scale = inScale;
	Invalidate();
}

// Gets the matrix representing this transform (outMatrix is assumed to be identity!)
void TransformComponent::GetMatrix(Mat4& outMatrix)
{
	// If this transform isn't dirty (it hasn't moved), just move on
	if (!isDirty)
	{
		outMatrix = cachedMatrix;
		return;
	}

	outMatrix = Mat4::TranslateRotateScale(position, rotation, scale);

	// If we have a parent, apply that as well
	if (parent != nullptr)
	{
		Mat4 parent_matrix;
		parent->GetMatrix( parent_matrix );

		outMatrix = parent_matrix * outMatrix;
	}

	cachedMatrix = outMatrix;
	isDirty = false;
}

// Marks this transform as changed, so matrices and such will be updated
void TransformComponent::Invalidate()
{
	// Already invalid
	// Should we broadcast anyways?
	onInvalidated.Broadcast(this);
	isDirty = true;
}

// Attaches this transform to another transform, making all values relative
void TransformComponent::SetParent(TransformComponent* in_parent)
{
	if (parent != nullptr)
	{
		Error("Already has a parent, not implemented yet");
	}

	parent = in_parent;

	// Null-parent, move on
	if (parent == nullptr)
		return;

	parent->onInvalidated.AddObject(this, &TransformComponent::HandleParentInvalidated);
}