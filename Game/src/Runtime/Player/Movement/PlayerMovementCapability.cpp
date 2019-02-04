#include "PlayerMovementCapability.h"
#include "Core/Context/Context.h"
#include "Engine/Camera/CameraComponent.h"
#include "Engine/Transform/TransformComponent.h"
#include "Engine/Collision/ColliderComponent.h"
#include "Engine/Debug/Debug.h"
#include "Runtime/Movement/MovementComponent.h"
#include "Runtime/Player/Player.h"

void PlayerMovementCapability::Setup()
{
	movement	= GetOwner()->GetOrAddComponent<MovementComponent>();
	player 		= GetOwner()->GetOrAddComponent<PlayerComponent>();
	transform	= GetOwner()->GetOrAddComponent<TransformComponent>();
}

void PlayerMovementCapability::Tick()
{
	// Process movement hits to see if it affects our velocity
	for(const OverlapResult& hit : movement->hits)
	{
		// If we land on something with a mostly-positive normal, consider it ground
		if (Vec::Dot(hit.normal, Vec3::up) > 0.9f && vertical_velocity < 0.f)
			vertical_velocity = 0.f;
	}

	InputState& input = gContext->input;
	float input_h = 0.f, input_v = 0.f;

	Vec2 dir;

	if (input.GetKey(Key::D))
		dir.x += 1.f;
	if (input.GetKey(Key::A))
		dir.x -= 1.f;
	if (input.GetKey(Key::W))
		dir.y += 1.f;
	if (input.GetKey(Key::S))
		dir.y -= 1.f;

	// Normalize input
	if (!Vec::NearlyZero(dir))
		dir = Vec::Normalize(dir);

	// Get movement vectors
	Vec3 forward(0.f, 0.f, -1.f);
	Vec3 right(1.f, 0.f, 0.f);

	if (player->follower_camera != nullptr)
	{
		forward = player->follower_camera->forward;
		forward = Vec::ConstrainToPlane(forward, Vec3::up);
		right = Vec::Cross(forward, Vec3::up);
	}

	movement->Translate((right * dir.x + forward * dir.y) * movement_speed * Time::Delta());

	// Gravity
	vertical_velocity -= 20.f * Time::Delta();
	if (input.GetKeyPressed(Key::Spacebar))
	{
		vertical_velocity = 10.f;
	}

	if (input.GetKey(Key::R))
		movement->Translate(Vec3::up * 2.f * Time::Delta());
	if (input.GetKey(Key::F))
		movement->Translate(Vec3::up * -2.f * Time::Delta());

	movement->Translate(Vec3::up * vertical_velocity * Time::Delta());
}
