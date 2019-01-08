
#include "Reflection.h"
#include "Type.h"

uint32 Reflection::lastId = 0;
Array<Type*> Reflection::registeredTypes;
Map<String, Type*> Reflection::nameLookup;

Type* Reflection::Get(const char* name)
{
	Type* type = nameLookup[name];

	if (type == nullptr)
		Error("Failed to find type '%s'", name);

	return type;
}