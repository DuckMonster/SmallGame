#include "CollisionSystem.h"
#include "Engine/Transform/TransformComponent.h"
#include "Engine/Debug/Debug.h"

void CollisionSystem::Setup()
{

}

void CollisionSystem::ProcessEntity(Entity* entity, ColliderComponent* collider)
{
	// Update matrix of collider object, if there is a transform
	auto transform = entity->GetComponent<TransformComponent>();
	if (transform != nullptr)
	{
		collider->object->transform = transform->GetMatrix();
	}

	// Activate colliders
	collider->object->is_active = true;
} 

void CollisionSystem::RunEnd()
{
	// Update overlaps
	CollisionScene* scene = GetScene()->GetCollisionScene();
	//scene->UpdateOverlaps();
}