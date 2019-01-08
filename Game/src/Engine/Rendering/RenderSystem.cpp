#include "RenderSystem.h"
#include "Core/Scene/Scene.h"
#include "Engine/Camera/CameraComponent.h"
#include "RenderStaticComponent.h"

void RenderSystem::Setup()
{
	pipeline.Create();
}

void RenderSystem::RunBegin()
{
	auto renderComp = GetScene()->GetStaticComponent<RenderStaticComponent>();
	renderComp->activeCamera->GetMatrix(frameScene.camera.matrix);
}

void RenderSystem::ProcessEntity(Entity* entity, RenderableComponent* renderable, TransformComponent* transform)
{
	RenderObject& object = frameScene.objects[frameScene.numObjects];
	object.mesh = *renderable->mesh;
	object.material = *renderable->material;
	object.matrix = Mat4(1.f);
	transform->GetMatrix(object.matrix);

	object.passFlags = RP_Geometry | RP_Shadow;
	frameScene.numObjects++;
}

void RenderSystem::RunEnd()
{
	pipeline.Execute(frameScene);
	frameScene.numObjects = 0;
}
