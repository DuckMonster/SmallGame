#include "Collision.h"
#include "Engine/Debug/Debug.h"

struct OverlapResultPair
{
	// Makes a result pair
	static OverlapResultPair Make(
		bool was_overlap,
		ColliderObject* obj_a, ColliderObject* obj_b,
		const Vec3& a_normal, float penetration_depth)
	{
		OverlapResultPair result;
		result.a.was_overlap = result.b.was_overlap = was_overlap;

		result.a.other = obj_b;
		result.a.normal = a_normal;
		result.a.penetration_depth = penetration_depth;

		result.b.other = obj_a;
		result.b.normal = -a_normal; // b's normal will the inverse of a's normal
		result.b.penetration_depth = penetration_depth;

		return result;
	}

	OverlapResult a;
	OverlapResult b;

	OverlapResultPair& Swap()
	{
		OverlapResult temp = b;
		b = a;
		a = temp;

		return *this;
	}
};

namespace
{
	// Sphere-sphere overlap
	OverlapResultPair Overlap_SphereSphere(
		ColliderObject* obj_a, const SphereCollider* col_a,
		ColliderObject* obj_b, const SphereCollider* col_b)
	{
		Sphere sph_a = Sphere::Transform(col_a->sphere, obj_a->transform);
		Sphere sph_b = Sphere::Transform(col_b->sphere, obj_b->transform);

		float dist = Vec::Distance(sph_a.origin, sph_b.origin);
		float rad_sum = sph_a.radius + sph_b.radius;

		float pen_depth = rad_sum - dist;
		Vec3 normal = Vec::Normalize(sph_a.origin - sph_b.origin);

		// Make result
		return OverlapResultPair::Make(
			pen_depth > 0.f,
			obj_a, obj_b,
			normal, pen_depth
		);
	}

	// Box-sphere overlap
	OverlapResultPair Overlap_BoxSphere(
		ColliderObject* obj_a, const BoxCollider* col_a,
		ColliderObject* obj_b, const SphereCollider* col_b)
	{
		Box box = Box::Transform(col_a->box, obj_a->transform);
		Sphere sphere = Sphere::Transform(col_b->sphere, obj_b->transform);

		// Get the basis vectors of the box
		Vec3 x = box.mat[0];
		float x_len = Vec::Length(x);
		x /= x_len;

		Vec3 y = box.mat[1];
		float y_len = Vec::Length(y);
		y /= y_len;

		Vec3 z = box.mat[2];
		float z_len = Vec::Length(z);
		z /= z_len;

		// Origin relative to box
		Vec3 s_origin = sphere.origin - Vec3(box.mat[3]);
		float s_radius = sphere.radius;

		Vec3 p = Vec3::zero;

		// Find the closest position by clamping the spheres position onto the
		//		basis vectors of the box
		{
			float x_dist = Vec::Dot(s_origin, x);
			p += x * Math::Clamp(x_dist, -x_len / 2.f, x_len / 2.f);
		}
		{
			float y_dist = Vec::Dot(s_origin, y);
			p += y * Math::Clamp(y_dist, -y_len / 2.f, y_len / 2.f);
		}
		{
			float z_dist = Vec::Dot(s_origin, z);
			p += z * Math::Clamp(z_dist, -z_len / 2.f, z_len / 2.f);
		}

		// The normal of the penetration will be the direction towards the closest point
		Vec3 normal = p - s_origin;
		float pen_depth = s_radius - Vec::Length(normal);
		normal = Vec::Normalize(normal);

		return OverlapResultPair::Make(
			Vec::DistanceSqrd(p, s_origin) < (s_radius * s_radius),
			obj_a, obj_b,
			normal, pen_depth
		);
	}

	// Box-box overlap
	OverlapResultPair Overlap_BoxBox(
		ColliderObject* obj_a, const BoxCollider* col_a,
		ColliderObject* obj_b, const BoxCollider* col_b)
	{
		Box box_a = Box::Transform(col_a->box, obj_a->transform);
		Box box_b = Box::Transform(col_b->box, obj_b->transform);

		// Get the basis vectors of the cubes, which we will use to
		// find the separating axis
		Vec3 a_x = Vec::Normalize((Vec3)box_a.mat[0]);
		Vec3 a_y = Vec::Normalize((Vec3)box_a.mat[1]);
		Vec3 a_z = Vec::Normalize((Vec3)box_a.mat[2]);

		Vec3 b_x = Vec::Normalize((Vec3)box_b.mat[0]);
		Vec3 b_y = Vec::Normalize((Vec3)box_b.mat[1]);
		Vec3 b_z = Vec::Normalize((Vec3)box_b.mat[2]);

		// Edge-to-edge cases have to be covered by these axes
		Vec3 xx = Vec::Normalize(Vec::Cross(a_x, b_x));
		Vec3 xy = Vec::Normalize(Vec::Cross(a_x, b_y));
		Vec3 xz = Vec::Normalize(Vec::Cross(a_x, b_z));
		Vec3 yx = Vec::Normalize(Vec::Cross(a_y, b_x));
		Vec3 yy = Vec::Normalize(Vec::Cross(a_y, b_y));
		Vec3 yz = Vec::Normalize(Vec::Cross(a_y, b_z));
		Vec3 zx = Vec::Normalize(Vec::Cross(a_z, b_x));
		Vec3 zy = Vec::Normalize(Vec::Cross(a_z, b_y));
		Vec3 zz = Vec::Normalize(Vec::Cross(a_z, b_z));

		Vec3 a_verts[8] = {
			Vec3(-0.5f, -0.5f, -0.5f),
			Vec3(0.5f, -0.5f, -0.5f),
			Vec3(-0.5f, 0.5f, -0.5f),
			Vec3(0.5f, 0.5f, -0.5f),

			Vec3(-0.5f, -0.5f, 0.5f),
			Vec3(0.5f, -0.5f, 0.5f),
			Vec3(-0.5f, 0.5f, 0.5f),
			Vec3(0.5f, 0.5f, 0.5f),
		};
		Vec3 b_verts[8] = {
			Vec3(-0.5f, -0.5f, -0.5f),
			Vec3(0.5f, -0.5f, -0.5f),
			Vec3(-0.5f, 0.5f, -0.5f),
			Vec3(0.5f, 0.5f, -0.5f),

			Vec3(-0.5f, -0.5f, 0.5f),
			Vec3(0.5f, -0.5f, 0.5f),
			Vec3(-0.5f, 0.5f, 0.5f),
			Vec3(0.5f, 0.5f, 0.5f),
		};

		// Transform all the vertices of the cubes
		for(uint32 i=0; i<8; ++i)
		{
			a_verts[i] = box_a.mat * a_verts[i];
			b_verts[i] = box_b.mat * b_verts[i];
		}

		// Helper struct containing the bounds of a projection onto a normal
		struct Projection
		{
			Projection(const Vec3& normal, const Vec3 verts[8])
			{
				min = BIG_NUMBER;
				max = -BIG_NUMBER;

				// Project all vertices onto the normal, and record the min-max
				for(uint32 i=0; i<8; ++i)
				{
					float dot = Vec::Dot(normal, verts[i]);
					if (dot < min)
						min = dot;
					if (dot > max)
						max = dot;
				}
			}
			float GetPenetration(const Projection& other)
			{
				return max - other.min;
			}
			float Middle()
			{
				return (min + max) * 0.5f;
			}
			float min;
			float max;
		};

		float pen_min = BIG_NUMBER;
		Vec3 pen_min_normal = Vec3::zero;

		// Lambda to test a specific axis, returns if the axis was separating
		auto test_axis = [&](const Vec3& axis)
		{
			// Don't do separation checking on 0-axis
			if (Vec::NearlyZero(axis))
				return;

			Projection a_proj(axis, a_verts),
				b_proj(axis, b_verts);

			float pen_a = a_proj.GetPenetration(b_proj);
			float pen_b = b_proj.GetPenetration(a_proj);
			float pen_min_cur = Math::Min(pen_a, pen_b);

			// one of the projections don't penetrate, so they're separated
			if (pen_min_cur < pen_min)
			{
				float dir = a_proj.Middle() - b_proj.Middle();
				pen_min = pen_min_cur;
				pen_min_normal = axis * Math::Sign(dir);
			}
		};

		// Test all axes! (Which is just the basis-vectors of both cubes, and the edge-normals
		test_axis(a_x);
		test_axis(a_y);
		test_axis(a_z);

		test_axis(b_x);
		test_axis(b_y);
		test_axis(b_z);

		test_axis(xx);
		test_axis(xy);
		test_axis(xz);
		test_axis(yx);
		test_axis(yy);
		test_axis(yz);
		test_axis(zx);
		test_axis(zy);
		test_axis(zz);

		if (pen_min > 0.f)
		{
			// If there wasn't a separating axis, calculate normal and the such
			Vec3 normal = Vec::Normalize(pen_min_normal);

			return OverlapResultPair::Make(
				true,
				obj_a, obj_b,
				normal, pen_min
			);
		}
		else
		{
			// Otherwise, just return zero stuff
			return OverlapResultPair::Make(
				false,
				obj_a, obj_b,
				Vec3::zero, 0.f
			);
		}
	}

	// Base-base overlap (will find and call the appropriate overlap function)
	OverlapResultPair Overlap_Base(
		ColliderObject* obj_a, const ColliderBase* col_a,
		ColliderObject* obj_b, const ColliderBase* col_b)
	{
		switch(col_a->type)
		{
			// Sphere - ?
			case ColliderType::Sphere:
			{
				const SphereCollider* sph_a = (SphereCollider*)col_a;
				switch(col_b->type)
				{
					// Sphere - Sphere
					case ColliderType::Sphere:
					{
						const SphereCollider* sph_b = (SphereCollider*)col_b;
						return Overlap_SphereSphere(obj_a, sph_a, obj_b, sph_b);
					} break;

				// Box - Box
					case ColliderType::Box:
					{
						const BoxCollider* box_b = (BoxCollider*)col_b;
						return Overlap_BoxSphere(obj_b, box_b, obj_a, sph_a).Swap();
					} break;
				}
			} break;

			// Box - ?
			case ColliderType::Box:
			{
				const BoxCollider* box_a = (BoxCollider*)col_a;
				switch(col_b->type)
				{
					// Box - Sphere
					case ColliderType::Sphere:
					{
						const SphereCollider* sph_b = (SphereCollider*)col_b;
						return Overlap_BoxSphere(obj_a, box_a, obj_b, sph_b);
					} break;

					// Box - Box
					case ColliderType::Box:
					{
						const BoxCollider* box_b = (BoxCollider*)col_b;
						return Overlap_BoxBox(obj_a, box_a, obj_b, box_b);
					} break;
				}
			} break;
		}

		return OverlapResultPair();
	}
}

ColliderObject* CollisionScene::CreateObject()
{
	ColliderObject* obj = new ColliderObject();
	objects.Add(obj);

	return obj;
}

void CollisionScene::DestroyObject(ColliderObject* obj)
{
#ifdef DEBUG
	Assert(objects.Contains(obj));
#endif

	objects.Remove(obj);
	delete obj;
}

// Queries the scene for hits when moving an object by some delta transform
void CollisionScene::QueryDeltaMovement(ColliderObject* obj, const Mat4& movement, TArray<OverlapResult>& out_hits)
{
	// Not active, we dont care
	if (!obj->is_active)
		return;

	Mat4 old_transform = obj->transform;
	obj->transform = movement * obj->transform;

	for(ColliderObject* other : objects)
	{
		if (other == obj)
			continue;

		// The other is not active, so ignore it
		if (!other->is_active)
			continue;

		for(ColliderBase* col_a : obj->colliders)
		{
			for(ColliderBase* col_b : other->colliders)
			{
				OverlapResultPair pair = Overlap_Base(obj, col_a, other, col_b);

				if (pair.a.was_overlap)
					out_hits.Add(pair.a);
			}
		}
	}

	obj->transform = old_transform;
}

void CollisionScene::UpdateOverlaps()
{
	for(ColliderObject* obj : objects)
	{
		obj->overlapped_this_frame = false;
	}

	for(uint32 i=0; i<objects.Size(); ++i)
	{
		ColliderObject* obj = objects[i];
		UpdateOverlapsFor(obj, i + 1);
	}
}

void CollisionScene::UpdateOverlapsFor(ColliderObject* obj, uint32 index_offset/* = 0*/)
{
	if (!obj->is_active)
		return;

	for(uint32 i=index_offset; i<objects.Size(); ++i)
	{
		ColliderObject* other = objects[i];

		if (!other->is_active)
			continue;

		UpdateOverlapsForPair(obj, other);
	}
}
void CollisionScene::UpdateOverlapsForPair(ColliderObject* obj_a, ColliderObject* obj_b)
{
	for(const ColliderBase* col_a : obj_a->colliders)
	{
		for(const ColliderBase* col_b: obj_b->colliders)
		{
			OverlapResultPair result = Overlap_Base(obj_a, col_a, obj_b, col_b);
			if (result.a.was_overlap)
			{
				obj_a->overlapped_this_frame = true;
				obj_a->on_overlap.Broadcast(result.a);

				obj_b->overlapped_this_frame = true;
				obj_b->on_overlap.Broadcast(result.b);
			}
		}
	}
}