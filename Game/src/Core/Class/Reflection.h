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
	static Array<Type*> registeredTypes;
	static Map<String, Type*> nameLookup;

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
		Type* type = Type::Create<T>(lastId, name);

		registeredTypes.Add(type);
		nameLookup[name] = type;
		TypeLookup<T>::type = type;

		return type;
	}

private:
	static uint32 lastId;
};

template<typename T>
Type* Reflection::TypeLookup<T>::type = nullptr;
