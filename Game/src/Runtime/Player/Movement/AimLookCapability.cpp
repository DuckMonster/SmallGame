
#include "AimLookCapability.h"
#include "Core/Math/Math.h"
#include "Engine/Transform/TransformComponent.h"
#include "Runtime/Player/Player.h"

void AimLookCapability::Setup()
{
	player		= GetOwner()->GetOrAddComponent<PlayerComponent>();
	transform	= player->mesh_entity->GetOrAddComponent<TransformComponent>();
}

void AimLookCapability::Tick()
{
	Vec3 direction = player->aim_world_position - transform->GetWorldPosition();
	float angle_target = Math::Atan2(direction.z, direction.x);
	float angle_delta = Math::GetDeltaAngleRadians(angle, angle_target);

	angle += angle_delta * 20.f * Time::Delta();
	angle = Math::UnwindRadians(angle);

	Quat quat = Quat::AngleAxis(-angle, Vec3::Up);
	transform->SetRotation(quat);
}
