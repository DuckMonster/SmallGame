
#include "Entity.h"
#include "Core/Class/Type.h"

Entity::~Entity()
{
	for(Component* comp : components)
	{
		comp->Destroy();
		delete comp;
}
}

Component* Entity::AddComponent(Type* type)
{
	Component* comp = NewObject<Component>(type);
	comp->SetOwner(this);
	components.Add(comp);

	comp->Setup();

	return comp;
}
