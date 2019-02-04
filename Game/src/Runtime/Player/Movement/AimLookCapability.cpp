
#include "AimLookCapability.h"
#include "Core/Math/Math.h"
#include "Engine/Transform/TransformComponent.h"
#include "Runtime/Movement/MovementComponent.h"
#include "Runtime/Player/Player.h"
#include "Engine/Debug/Debug.h"

void AimLookCapability::Setup()
{
	player		= GetOwner()->GetOrAddComponent<PlayerComponent>();
	transform	= GetOwner()->GetOrAddComponent<TransformComponent>();
	movement	= GetOwner()->GetOrAddComponent<MovementComponent>();
}
void AimLookCapability::Tick()
{
	Vec3 direction = player->aim_world_position - transform->GetWorldPosition();
	float angle_target = Math::Atan2(direction.z, direction.x);
	float angle_delta = Math::GetDeltaAngleRadians(angle, angle_target);

	angle += angle_delta * 20.f * Time::Delta();
	angle = Math::UnwindRadians(angle);

	Quat quat = Quat::AngleAxis(-angle, Vec3::up);
	movement->SetRotation(quat);
}
