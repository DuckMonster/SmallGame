#pragma once
#include "Core/Standard/Map.h"

class Type;

class Reflection
{
	// Template class to find type from template argument
	template<typename T>
	class TypeLookup
	{
	public:
		static Type* type;
	};

public:
	static Array<Type*> registered_types;
	static Map<String, Type*> name_lookup;

	template<typename T>
	static Type* Get()
	{
		return TypeLookup<T>::type;
	}

	static Type* Get(const char* name);

	template<typename T>
	static Type* RegisterType(const char* name)
	{
		Assert(TypeLookup<T>::type == nullptr);
		Type* type = Type::Create<T>(last_id, name);

		registered_types.Add(type);
		name_lookup[name] = type;
		TypeLookup<T>::type = type;

		return type;
	}

private:
	static uint32 last_id;
};

template<typename T>
Type* Reflection::TypeLookup<T>::type = nullptr;
