#include "CollisionSystem.h"
#include "Engine/Debug/Debug.h"

void CollisionSystem::Setup()
{

}

void CollisionSystem::Run()
{
	CollisionScene* scene = GetScene()->GetCollisionScene();

	// Update overlaps
	scene->UpdateOverlaps();

	for(ColliderObject* obj : scene->objects)
	{
		Color color = obj->overlapped_this_frame ?
			Color::red :
			Color::green;

		// Draw cubes
		for(BoxCollider& box : obj->boxes)
		{
			BoxCollider box_t = box.Transform(obj->transform);
			Debug::DrawCube(box_t.mat, color);
		}

		// Draw spheres
		for(SphereCollider& sphere : obj->spheres)
		{
			SphereCollider sph_t = sphere.Transform(obj->transform);
			Debug::DrawSphere(sph_t.origin, sph_t.radius, color);
		}
	}
}
