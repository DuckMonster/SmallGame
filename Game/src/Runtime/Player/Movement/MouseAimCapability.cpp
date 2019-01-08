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

	// Create a sphere to show where the mouse is
	sphere = ShapePrefab::CreateSphere(gScene);
	sphere.entity->SetName("Aim-sphere");
	sphere.transform->SetScale(Vec3(0.2f));

	// Add some debug colliders to the aim-sphere
	{
		auto sphere_collider = sphere.entity->AddComponent<ColliderComponent>();
		sphere_collider->AddSphere(Vec3(0.f), 1.f);
	}

	collider->onOverlap.AddObject(this, &MouseAimCapability::HandleCollision);
}

void MouseAimCapability::Tick()
{
	CameraComponent* camera = gScene->GetStaticComponent<RenderStaticComponent>()->activeCamera;

	// Deproject mouse
	Mat4 invMatrix;
	camera->GetMatrixInv(invMatrix);

	Vec2 mouse(
		gContext->input.mouseState.x,
		gContext->input.mouseState.y
	);

	Ray ray = Camera::Deproject(mouse, invMatrix);
	Plane plane = Plane::FromPointNormal(transform->GetPosition() + Vec3::Up, Vec3::Up);


	// Project onto aim-plane
	static float hOffset = 0;
	hOffset += gContext->input.GetWheelDelta() * 0.2f;

	player->aimWorldPosition = ray.ProjectToPlane(plane);
	sphere.transform->SetPosition(player->aimWorldPosition);
	sphere.transform->SetScale(1.f + hOffset);
}

void MouseAimCapability::HandleCollision(Entity* other)
{
	Debug::PrintOneFrame("Collided with %s", *other->GetName());
}
