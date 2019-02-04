#pragma once
#include "TypeDefs.h"
#include "Reflection.h"
#include "Property.h"

class TypedObject;

class Type
{
	friend TypedObject* NewObject(Type* type);
	friend class Reflection;

	/* Function for creating typed objects */
	typedef TypedObject* (*ObjectCreateFunc)();
	template<typename T>
	static TypedObject* ObjectCreate()
	{
		return new T;
	}

public:
	/* Creates a type object */
	template<typename T>
	static Type* Create(uint32 id, const char* name)
	{
		Type* type = new Type(id, name);
		type->create_func = ObjectCreate<T>;

		return type;
	}

	template<typename T>
	static Type* Get() { return Reflection::Get<T>(); }
	static Type* Get(const char* name);

public:
	Type(uint32 id, const char* name) : id(id), name(name) {}

	const uint32 id;
	const String name;

	const Array<PropertyBase*>& GetProperties() const { return properties; }
	Array<PropertyBase*> properties;

private:
	ObjectCreateFunc create_func;
};

TypedObject* NewObject(Type* type);
template<typename T>
T* NewObject(Type* type) { return (T*)NewObject(type); }
template<typename T>
T* NewObject() { return (T*)NewObject(Type::Get<T>()); }