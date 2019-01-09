#include "Collision.h"
#include "Engine/Debug/Debug.h"

namespace
{
	// Sphere-sphere overlap
	bool Overlap_SphereSphere(const SphereCollider& a, const SphereCollider& b)
	{
		float dist_sqrd = Vec::DistanceSqrd(a.origin, b.origin);
		float rad_sqrd = a.radius + b.radius;
		rad_sqrd = rad_sqrd * rad_sqrd;

		return dist_sqrd <= rad_sqrd;
	}

	// Box-sphere overlap
	bool Overlap_BoxSphere(const BoxCollider& box, const SphereCollider& sphere)
	{
		Vec3 x = box.mat[0];
		float x_len = Vec::Length(x);
		x /= x_len;

		Vec3 y = box.mat[1];
		float y_len = Vec::Length(y);
		y /= y_len;

		Vec3 z = box.mat[2];
		float z_len = Vec::Length(z);
		z /= z_len;

		Vec3 p = box.mat[3];
		Vec3 s_origin = sphere.origin - p;
		p = Vec3(0.f);

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

		return Vec::DistanceSqrd(p, s_origin) < (sphere.radius * sphere.radius);
	}

	// Box-box overlap
	bool Overlap_BoxBox(const BoxCollider& a, const BoxCollider& b)
	{
		// Get the basis vectors of the cubes, which we will use to
		// find the separating axis
		Vec3 a_x = a.mat[0];
		Vec3 a_y = a.mat[1];
		Vec3 a_z = a.mat[2];

		Vec3 b_x = b.mat[0];
		Vec3 b_y = b.mat[1];
		Vec3 b_z = b.mat[2];

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
			a_verts[i] = a.mat * a_verts[i];
			b_verts[i] = b.mat * b_verts[i];
		}

		// Helper struct containing the bounds of a projection onto a normal
		struct Projection
		{
			Projection(Vec3 normal, Vec3 verts[8])
			{
				min = BIG_NUMBER;
				max = -BIG_NUMBER;

				for(uint32 i=0; i<8; ++i)
				{
					float dot = Vec::Dot(normal, verts[i]);
					if (dot < min)
						min = dot;
					if (dot > max)
						max = dot;
				}
			}
			bool Overlaps(const Projection& other)
			{
				return max > other.min &&
					other.max > min;
			}
			float min;
			float max;
		};

		// Project on a's axes
		{
			Projection a_proj(a_x, a_verts),
				b_proj(a_x, b_verts);

			if (!a_proj.Overlaps(b_proj))
				return false;
		}
		{
			Projection a_proj(a_y, a_verts),
				b_proj(a_y, b_verts);

			if (!a_proj.Overlaps(b_proj))
				return false;
		}
		{
			Projection a_proj(a_z, a_verts),
				b_proj(a_z, b_verts);

			if (!a_proj.Overlaps(b_proj))
				return false;
		}

		// Project on b's axes
		{
			Projection a_proj(b_x, a_verts),
				b_proj(b_x, b_verts);

			if (!a_proj.Overlaps(b_proj))
				return false;
		}
		{
			Projection a_proj(b_y, a_verts),
				b_proj(b_y, b_verts);

			if (!a_proj.Overlaps(b_proj))
				return false;
		}
		{
			Projection a_proj(b_z, a_verts),
				b_proj(b_z, b_verts);

			if (!a_proj.Overlaps(b_proj))
				return false;
		}

		return true;
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
	for(uint32 i=index_offset; i<objects.Size(); ++i)
	{
		ColliderObject* other = objects[i];
		UpdateOverlapsForPair(obj, other);
	}
}
void CollisionScene::UpdateOverlapsForPair(ColliderObject* obj_a, ColliderObject* obj_b)
{
	// Sphere - ?
	for(const SphereCollider& a : obj_a->spheres)
	{
		// Transform!
		SphereCollider a_t = a.Transform(obj_a->transform);

		// Sphere - Sphere
		for(const SphereCollider& b : obj_b->spheres)
		{
			// Transform!
			SphereCollider b_t = b.Transform(obj_b->transform);

			if (Overlap_SphereSphere(a_t, b_t))
			{
				obj_a->overlapped_this_frame = true;
				obj_a->on_overlap.Broadcast(obj_b);
				obj_b->overlapped_this_frame = true;
				obj_b->on_overlap.Broadcast(obj_a);
				return;
			}
		}

		// Sphere - Box
		for(const BoxCollider& b : obj_b->boxes)
		{
			// Transform!
			BoxCollider b_t = b.Transform(obj_b->transform);

			if (Overlap_BoxSphere(b_t, a_t))
			{
				obj_a->overlapped_this_frame = true;
				obj_a->on_overlap.Broadcast(obj_b);
				obj_b->overlapped_this_frame = true;
				obj_b->on_overlap.Broadcast(obj_a);
				return;
			}
		}
	}

	// Box - ?
	for(const BoxCollider& a : obj_a->boxes)
	{
		// Transform!
		BoxCollider a_t = a.Transform(obj_a->transform);

		// Box - Sphere
		for(const SphereCollider& b : obj_b->spheres)
		{
			// Transform!
			SphereCollider b_t = b.Transform(obj_b->transform);

			if (Overlap_BoxSphere(a_t, b_t))
			{
				obj_a->overlapped_this_frame = true;
				obj_a->on_overlap.Broadcast(obj_b);
				obj_b->overlapped_this_frame = true;
				obj_b->on_overlap.Broadcast(obj_a);
				return;
			}
		}

		// Box - Box
		for(const BoxCollider& b : obj_b->boxes)
		{
			// Transform!
			BoxCollider b_t = b.Transform(obj_b->transform);

			if (Overlap_BoxBox(a_t, b_t))
			{
				obj_a->overlapped_this_frame = true;
				obj_a->on_overlap.Broadcast(obj_b);
				obj_b->overlapped_this_frame = true;
				obj_b->on_overlap.Broadcast(obj_a);
				return;
			}
		}
	}
}