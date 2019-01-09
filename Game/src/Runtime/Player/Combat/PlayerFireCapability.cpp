
#include "PlayerFireCapability.h"
#include "Core/Context/Context.h"
#include "Core/Scene/Scene.h"
#include "Engine/Transform/TransformComponent.h"
#include "Runtime/Projectile/Projectile.h"
#include "Runtime/Player/Player.h"

void PlayerFireCapability::Setup()
{
	transform = GetOwner()->GetOrAddComponent<TransformComponent>();
	player = GetOwner()->GetOrAddComponent<PlayerComponent>();
}

void PlayerFireCapability::Tick()
{
	const InputState& input = gContext->input;

	if (input.GetMouseButtonPressed(MouseButton::Left))
	{
		Vec3 origin = transform->GetPosition() + Vec3(0.f, 1.f, 0.f);
		Vec3 direction = player->aim_world_position - origin;
		ProjectilePrefab::Create(gScene, origin, direction);
	}
}
