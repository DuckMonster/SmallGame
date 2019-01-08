
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
	Vec3 direction = player->aimWorldPosition - transform->GetWorldPosition();
	float targetAngle = Math::Atan2(direction.z, direction.x);
	float deltaAngle = Math::GetDeltaAngleRadians(angle, targetAngle);

	angle += deltaAngle * 20.f * Time::Delta();
	angle = Math::UnwindRadians(angle);

	Quat quat = Quat::AngleAxis(-angle, Vec3::Up);
	transform->SetRotation(quat);
}
