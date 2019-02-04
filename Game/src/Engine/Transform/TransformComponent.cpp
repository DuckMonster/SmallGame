
#include "Core/Class/TypeHelpers.h"
#include "Engine/Debug/Debug.h"
#include "TransformComponent.h"

REGISTER_TYPE(TransformComponent);
REGISTER_PROP(TransformComponent, transform);

// Gets the world-space position of this transform
Vec3 TransformComponent::GetWorldPosition() const
{
	if (parent == nullptr)
		return transform.position;

	return parent->GetMatrix() * transform.position;
}

// Sets the position of this transform
void TransformComponent::SetPosition(const Vec3& in_position)
{
	transform.position = in_position;
	Invalidate();
}
// Translates this transform by some delta location
void TransformComponent::Translate(const Vec3& delta)
{
	transform.position += delta;
	Invalidate();
}

// Gets the world-space rotation
Quat TransformComponent::GetWorldRotation() const
{
	if (parent == nullptr)
		return transform.rotation;

	return transform.rotation * parent->transform.rotation;
}

// Sets the rotation of this transform
void TransformComponent::SetRotation(const Quat& in_rotation)
{
	transform.rotation = in_rotation;
	Invalidate();
}
// Rotates this transform by some delta
void TransformComponent::Rotate(const Quat& delta)
{
#ifdef DEBUG
	if (!Quat::IsUnit(delta))
	{
		Debug::PrintOneFrame("Rotating by non-unit quaternion (length: %f)", Quat::Length(delta));
	}
#endif

	// Since rotation multiplication are right-to-left (so right-most is applied first)
	//		we want to do our current rotation _first_, then the delta
	transform.rotation = Quat::Normalize(delta * transform.rotation);
	Invalidate();
}

// Sets the scale of this transform
void TransformComponent::SetScale(const Vec3& in_scale)
{
	transform.scale = in_scale;
	Invalidate();
}

// Gets the matrix representing this transform (out_matrix is assumed to be identity!)
Mat4 TransformComponent::GetMatrix()
{
	// If this transform isn't dirty (it hasn't moved), just move on
	if (!is_dirty)
		return cached_matrix;

	Mat4 result = transform.Matrix();

	// If we have a parent, apply that as well
	if (parent != nullptr)
		result = parent->GetMatrix() * result;

	cached_matrix = result;
	return result;
}

// Marks this transform as changed, so matrices and such will be updated
void TransformComponent::Invalidate()
{
	// Already invalid
	// Should we broadcast anyways?
	on_invalidated.Broadcast(this);
	is_dirty = true;
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

	parent->on_invalidated.AddObject(this, &TransformComponent::HandleParentInvalidated);
}