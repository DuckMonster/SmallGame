
#include "Reflection.h"
#include "Type.h"

uint32 Reflection::last_id = 0;
Array<Type*> Reflection::registered_types;
Map<String, Type*> Reflection::name_lookup;

Type* Reflection::Get(const char* name)
{
	Type* type = name_lookup[name];

	if (type == nullptr)
		Error("Failed to find type '%s'", name);

	return type;
}