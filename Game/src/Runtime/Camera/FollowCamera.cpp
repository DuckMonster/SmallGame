
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
	auto player_comp = target->GetOrAddComponent<PlayerComponent>();
	player_comp->follower_camera = result.camera;

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
		rotate_angle -= Time::Delta() * 1.6f;
	}
	if (input.GetKey(Key::Q))
	{
		rotate_angle += Time::Delta() * 1.6f;
	}

	Quat rotation = Quat::AngleAxis(rotate_angle, Vec3::up);

	// Translation
	if (input.GetKey(Key::Z))
	{
		zoom -= 0.4f * Time::Delta();
	}
	if (input.GetKey(Key::X))
	{
		zoom += 0.4f * Time::Delta();
	}

	Vec3 target_position = follow->target->GetPosition();
	root = Vec::Lerp(root, target_position, 5.f * Time::Delta());

	Vec3 rotated_offset = rotation * offset;
	camera->position = root + rotated_offset * offset_distance * zoom;
	camera->forward = -rotated_offset;
}
