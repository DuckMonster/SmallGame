#pragma once
#include "Primitives/Primitives.h"

class Entity;
class ColliderObject;

/* Struct encapsulating an overlap result */
struct OverlapResult
{
	bool was_overlap = false;
	ColliderObject* other = nullptr;
	Vec3 normal = Vec3(0.f);
	float penetration_depth = 0.f;
};

enum class ColliderType
{
	Invalid,
	Box,
	Sphere
};

struct ColliderObject;

/* Base class for a collider that can be attached to a CollisionObject */
struct ColliderBase
{
	ColliderBase(ColliderType type) : type(type) {}

	// The type of collider this is (for casting from base to derived)
	const ColliderType type;
};

/* A collider representing a box, which can be rotated at any angle */
struct BoxCollider : public ColliderBase
{
	BoxCollider() : ColliderBase(ColliderType::Box) {}
	Box box;
};

/* A collider representing a perfect sphere, with an origin and radius */
struct SphereCollider : public ColliderBase
{
	SphereCollider() : ColliderBase(ColliderType::Sphere) {}
	Sphere sphere;
};

/* Represents an object with any number of colliders attached
	and a transform */
struct ColliderObject
{
	// OnOverlapEvent(ColliderObject* other)
	typedef Event<const OverlapResult&> OnOverlapEvent;

	// The entity that this object may represent
	Entity* owner = nullptr;

	// The transform of this object
	Mat4 transform = Mat4(1.f);

	// Called when this collider overlaps with anything else
	OnOverlapEvent on_overlap;
	bool overlapped_this_frame = false;

	// If its not active, its not expected to interact with anything
	bool is_active = false;

	// Helper functions to add colliders
	void AddSphere(const Vec3& origin, float radius)
	{
		SphereCollider* sphere = new SphereCollider();
		sphere->sphere.origin = origin;
		sphere->sphere.radius = radius;

		colliders.Add(sphere);
	}
	void AddBox(const Vec3& position, const Vec3& size, const Quat& rotation)
	{
		BoxCollider* box = new BoxCollider();
		box->box = Box::Make(position, size, rotation);

		colliders.Add(box);
	}

	Array<ColliderBase*> colliders;
};

/* Separate scene that handles collision between collisionobjects */
class CollisionScene
{
public:
	ColliderObject* CreateObject();
	void DestroyObject(ColliderObject* obj);

	// Queries the scene for hits when moving an object by some delta transform
	void QueryDeltaMovement(ColliderObject* obj, const Mat4& movement, TArray<OverlapResult>& out_hits);

	void UpdateOverlaps();
	void UpdateOverlapsFor(ColliderObject* obj, uint32 index_offset = 0);
	void UpdateOverlapsForPair(ColliderObject* a, ColliderObject* b);

	Array<ColliderObject*> objects;
};