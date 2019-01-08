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
		Color color = obj->overlappedThisFrame ?
			Color::red :
			Color::green;

		// Draw cubes
		for(BoxCollider& box : obj->boxes)
		{
			BoxCollider transformedBox = box.Transform(obj->transform);
			Debug::DrawCube(transformedBox.mat, color);
		}

		// Draw spheres
		for(SphereCollider& sphere : obj->spheres)
		{
			SphereCollider transformedSphere = sphere.Transform(obj->transform);
			Debug::DrawSphere(transformedSphere.origin, transformedSphere.radius, color);
		}
	}
}
