#pragma once
#include "Core/System/System.h"
#include "Engine/Transform/TransformComponent.h"
#include "RenderableComponent.h"
#include "RenderData.h"
#include "RenderPipeline.h"

class RenderSystem : public ComponentSystem<RenderableComponent, TransformComponent>
{
public:
	void Setup() override;
	void RunBegin() override;
	void ProcessEntity(Entity* entity, RenderableComponent* renderable, TransformComponent* transform) override;
	void RunEnd() override;

private:
	RenderPipeline pipeline;
	RenderScene frameScene;
};