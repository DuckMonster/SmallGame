
#include "FollowCamera.h"
#include "Core/Scene/Scene.h"
#include "Core/Entity/Entity.h"
#include "Core/Class/TypeHelpers.h"
#include "Engine/Capability/CapabilityStatics.h"
#include "Engine/Transform/TransformComponent.h"
#include "Engine/Camera/CameraComponent.h"
#include "Runtime/Player/Player.h"

REGISTER_TYPE(FollowCameraComponent);
REGISTER_PROP(FollowCameraComponent, target);

// PREFAB
FollowCamera FollowCamera::Create(Scene* scene, Entity* target)
{
	FollowCamera result;
	result.entity = scene->CreateEntity("FollowCamera");
	result.component = result.entity->AddComponent<FollowCameraComponent>();
	result.camera = result.entity->AddComponent<CameraComponent>();

	result.component->SetTarget(target);

	AddCapability<FollowCameraCapability>(result.entity);

	// Tell the entity that we're following them
	auto playerComp = target->GetOrAddComponent<PlayerComponent>();
	playerComp->followerCamera = result.camera;

	return result;
}

// COMPONENT
void FollowCameraComponent::SetTarget(Entity* entity)
{
	target = entity->GetOrAddComponent<TransformComponent>();
}

// CAPABILITY
void FollowCameraCapability::Setup()
{
	follow = GetOwner()->GetOrAddComponent<FollowCameraComponent>();
	camera = GetOwner()->GetOrAddComponent<CameraComponent>();

	camera->forward = -offset;
}

void FollowCameraCapability::Tick()
{
	if (follow->target == nullptr)
		return;

	const InputState& input = gContext->input;
	// Rotation
	if (input.GetKey(Key::E))
	{
		rotateAngle -= Time::Delta() * 1.6f;
	}
	if (input.GetKey(Key::Q))
	{
		rotateAngle += Time::Delta() * 1.6f;
	}

	Quat rotation = Quat::AngleAxis(rotateAngle, Vec3::Up);

	// Translation
	if (input.GetKey(Key::Z))
	{
		zoom -= 0.4f * Time::Delta();
	}
	if (input.GetKey(Key::X))
	{
		zoom += 0.4f * Time::Delta();
	}

	Vec3 targetPosition = follow->target->GetPosition();
	root = Vec::Lerp(root, targetPosition, 5.f * Time::Delta());

	Vec3 rotatedOffset = rotation * offset;
	camera->position = root + rotatedOffset * offsetDistance * zoom;
	camera->forward = -rotatedOffset;
}