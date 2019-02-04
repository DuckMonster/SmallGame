
#include "Type.h"
#include "TypedObject.h"

Type* Type::Get(const char* name)
{
	return Reflection::Get(name);
}

TypedObject* NewObject(Type* type)
{
	if (type == nullptr)
	{
		Error("Tried to create object with null-type");
		return nullptr;
	}

	TypedObject* obj = type->create_func();
	obj->type = type;

	return obj;
}