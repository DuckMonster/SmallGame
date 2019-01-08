#include "PlayerMovementCapability.h"
#include "Core/Context/Context.h"
#include "Engine/Camera/CameraComponent.h"
#include "Engine/Transform/TransformComponent.h"
#include "Runtime/Player/Player.h"

void PlayerMovementCapability::Setup()
{
	transform 	= GetOwner()->GetOrAddComponent<TransformComponent>();
	player 		= GetOwner()->GetOrAddComponent<PlayerComponent>();
}

void PlayerMovementCapability::Tick()
{
	InputState& input = gContext->input;
	float hInput = 0.f, vInput = 0.f;

	Vec2 dir;

	if (input.GetKey(Key::D))
		dir.x += 1.f;
	if (input.GetKey(Key::A))
		dir.x -= 1.f;
	if (input.GetKey(Key::W))
		dir.y += 1.f;
	if (input.GetKey(Key::S))
		dir.y -= 1.f;

	static float temp_scale = 1.f;
	if (input.GetKey(Key::F))
		temp_scale -= Time::Delta();
	if (input.GetKey(Key::R))
		temp_scale += Time::Delta();

	transform->SetScale(temp_scale);

	// No movement
	if (dir.x == 0.f && dir.y == 0.f)
		return;

	// Get movement vectors
	Vec3 forward(0.f, 0.f, -1.f);
	Vec3 right(1.f, 0.f, 0.f);

	if (player->followerCamera != nullptr)
	{
		forward = player->followerCamera->forward;
		forward = Vec::ConstrainToPlane(forward, Vec3::Up);
		right = Vec::Cross(forward, Vec3::Up);
	}

	transform->Translate(( right * dir.x + forward * dir.y ) * movementSpeed * Time::Delta());
}