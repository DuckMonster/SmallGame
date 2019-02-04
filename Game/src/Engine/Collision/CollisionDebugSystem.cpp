#include "CollisionDebugSystem.h"
#include "Engine/Debug/Debug.h"
#include "Core/Context/Context.h"

void CollisionDebugSystem::RunBegin()
{
	const InputState& input = gContext->input;
	if (input.GetKeyPressed(Key::F4))
		debug_enabled = !debug_enabled;
}

void CollisionDebugSystem::ProcessEntity(Entity* entity, ColliderComponent* collider)
{
	if (!debug_enabled)
		return;

	// Debug draw
	if (collider->debug_draw || true)
	{
		ColliderObject* obj = collider->object;

		Color color = obj->overlapped_this_frame ?
			Color::red :
			Color::green;

		for(ColliderBase* col : obj->colliders)
		{
			switch(col->type)
			{
				case ColliderType::Box:
				{
					Box box = Box::Transform(((BoxCollider*)col)->box, obj->transform);
					Debug::DrawCube(box.mat, color);
				} break;

				case ColliderType::Sphere:
				{
					Sphere sph = Sphere::Transform(((SphereCollider*)col)->sphere, obj->transform);
					Debug::DrawSphere(sph.origin, sph.radius, color);
				} break;
			}
		}
	}
}