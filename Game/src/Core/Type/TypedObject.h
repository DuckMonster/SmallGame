#pragma once

class Type;

class TypedObject
{
	friend class Type;
	friend TypedObject* NewObject(Type*);

public:
	virtual ~TypedObject() {}
	Type* GetType() const
	{
		if (type == nullptr)
			Fatal("type is nullptr, was this object not created through NewObject?");

		return type;
	}

private:
	Type* type;
};