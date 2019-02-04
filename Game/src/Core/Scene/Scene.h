#pragma once
class SystemBase;
class CollisionScene;

class Scene
{
public:
	Scene();
	~Scene();
	void Tick();

	CollisionScene* GetCollisionScene() const { return collision_scene; }

private:
	CollisionScene* collision_scene;
};

extern Scene* scene;