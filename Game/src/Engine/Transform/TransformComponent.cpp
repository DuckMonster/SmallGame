
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
void TransformComponent::SetPosition(const Vec3& in_position)
{
	position = in_position;
	Invalidate();
}
// Translates this transform by some delta location
void TransformComponent::Translate(const Vec3& delta)
{
	position += delta;
	Invalidate();
}

// Sets the rotation of this transform
void TransformComponent::SetRotation(const Quat& in_rotation)
{
	rotation = in_rotation;
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
void TransformComponent::SetScale(const Vec3& in_scale)
{
	scale = in_scale;
	Invalidate();
}

// Gets the matrix representing this transform (out_matrix is assumed to be identity!)
void TransformComponent::GetMatrix(Mat4& out_matrix)
{
	// If this transform isn't dirty (it hasn't moved), just move on
	if (!is_dirty)
	{
		out_matrix = cached_matrix;
		return;
	}

	out_matrix = Mat4::TranslateRotateScale(position, rotation, scale);

	// If we have a parent, apply that as well
	if (parent != nullptr)
	{
		Mat4 parent_matrix;
		parent->GetMatrix( parent_matrix );

		out_matrix = parent_matrix * out_matrix;
	}

	cached_matrix = out_matrix;
	is_dirty = false;
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