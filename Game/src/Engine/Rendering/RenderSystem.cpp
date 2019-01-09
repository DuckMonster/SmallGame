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
	auto render_comp = GetScene()->GetStaticComponent<RenderStaticComponent>();
	render_comp->active_camera->GetMatrix(frame_scene.camera.matrix);
}

void RenderSystem::ProcessEntity(Entity* entity, RenderableComponent* renderable, TransformComponent* transform)
{
	RenderObject& object = frame_scene.objects[frame_scene.object_num];
	object.mesh = *renderable->mesh;
	object.material = *renderable->material;
	object.matrix = Mat4(1.f);
	transform->GetMatrix(object.matrix);

	object.pass_flag = RP_Geometry | RP_Shadow;
	frame_scene.object_num++;
}

void RenderSystem::RunEnd()
{
	pipeline.Execute(frame_scene);
	frame_scene.object_num = 0;
}
