#include "RenderSystem.h"
#include "Core/Scene/Scene.h"
#include "Engine/Camera/CameraComponent.h"
#include "Engine/Light/DirectionalLightComponent.h"
#include "RenderStaticComponent.h"

void RenderSystem::Setup()
{
	pipeline.Create();
}

void RenderSystem::RunBegin()
{
	auto render_comp = GetScene()->GetStaticComponent<RenderStaticComponent>();

	// Get camera
	render_comp->active_camera->GetMatrix(frame_scene.camera.matrix);

	// Get light source
	frame_scene.light.position = render_comp->light->position;
	frame_scene.light.direction = render_comp->light->direction;
}

void RenderSystem::ProcessEntity(Entity* entity, RenderableComponent* renderable, TransformComponent* transform)
{
	for(const RenderableComponent::Renderable& r : renderable->renderables)
	{
		RenderObject& object = frame_scene.objects[frame_scene.object_num];
		object.mesh = *r.mesh;
		object.material = *r.material;
		object.matrix = transform->GetMatrix();
		object.matrix = object.matrix * r.matrix;

		object.pass_flag = RP_Geometry | RP_Shadow;
		frame_scene.object_num++;
	}
}

void RenderSystem::RunEnd()
{
	pipeline.Execute(frame_scene);
	frame_scene.object_num = 0;
}
