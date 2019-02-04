#include "MouseAimCapability.h"
#include "Core/Math/Ray.h"
#include "Core/Scene/Scene.h"
#include "Engine/Shape/Shape.h"
#include "Engine/Camera/CameraComponent.h"
#include "Engine/Transform/TransformComponent.h"
#include "Engine/Rendering/RenderStaticComponent.h"
#include "Engine/Debug/Debug.h"
#include "Engine/Collision/ColliderComponent.h"
#include "Runtime/Camera/Camera.h"
#include "Runtime/Player/Player.h"

void MouseAimCapability::Setup()
{
	player = GetOwner()->GetOrAddComponent<PlayerComponent>();
	transform = GetOwner()->GetOrAddComponent<TransformComponent>();
	collider = GetOwner()->GetOrAddComponent<ColliderComponent>();
}

void MouseAimCapability::Tick()
{
	CameraComponent* camera = gScene->GetStaticComponent<RenderStaticComponent>()->active_camera;

	// Deproject mouse
	Mat4 matrix_inv;
	camera->GetMatrixInv(matrix_inv);

	Vec2 mouse(
		gContext->input.mouse_state.x,
		gContext->input.mouse_state.y
	);

	Ray ray = Camera::Deproject(mouse, matrix_inv);
	Plane plane = Plane::FromPointNormal(transform->GetPosition() + Vec3::up, Vec3::up);

	// Project onto aim-plane
	static float offset_h = 0;
	offset_h += gContext->input.GetWheelDelta() * 0.2f;

	player->aim_world_position = ray.ProjectToPlane(plane);
}