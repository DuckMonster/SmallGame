#pragma once
class Entity;

struct Collider
{

};

struct BoxCollider
{
	Mat4 mat;

	void Set(const Vec3& position, const Vec3& size, const Quat& rotation)
	{
		mat = Mat4::TranslateRotateScale(position, rotation, size);
	}
	BoxCollider Transform(const Mat4& transform) const
	{
		BoxCollider result;
		result.mat = transform * mat;

		return result;
	}
};

struct SphereCollider
{
	Vec3 origin;
	float radius;

	SphereCollider Transform(const Mat4& transform) const
	{ 
		SphereCollider result;
		result.origin = transform * origin;

		// Get the scaled radius by only transforming a vector of radius length,
		// 		we ignore translation by setting w to 0, so we only get scale and rotation.
		//		Then we get the length of that vector to find out how much it's scaled.
		float scaled_radius = Vec::Length(transform * Vec4(radius, 0.f, 0.f, 0.f));
		result.radius = scaled_radius;

		return result;
	}
};

struct ColliderObject
{
	// OnOverlapEvent(ColliderObject* other)
	typedef Event<ColliderObject*> OnOverlapEvent;

	// The entity that this object may represent
	Entity* owner = nullptr;

	// The transform of this object
	Mat4 transform = Mat4(1.f);

	Array<BoxCollider> boxes;
	Array<SphereCollider> spheres;

	// Called when this collider overlaps with anything else
	OnOverlapEvent on_overlap;
	bool overlapped_this_frame = false;
};

class CollisionScene
{
public:
	ColliderObject* CreateObject();
	void DestroyObject(ColliderObject* obj);

	void UpdateOverlaps();
	void UpdateOverlapsFor(ColliderObject* obj, uint32 index_offset = 0);
	void UpdateOverlapsForPair(ColliderObject* a, ColliderObject* b);

	Array<ColliderObject*> objects;
};