#pragma once
class Resource;
class Entity;
class Component;

namespace StringHelpers
{
	TString ToString(const int& value);
	TString ToString(const float& value);
	TString ToString(const Vec2& value);
	TString ToString(const Vec3& value);
	TString ToString(const Vec4& value);
	TString ToString(const Quat& value);
	TString ToString(const Color& value);
	TString ToString(const Resource* resource);
	TString ToString(const Entity* entity);
	TString ToString(const Component* component);
}

class TypedObject;

class PropertyBase
{
protected:
	PropertyBase(const char* name) : name(name) {}

public:
	virtual TString ValueString(const TypedObject* object) const = 0;
	const String name;
};

template<typename InType, typename InPropType>
class Property : public PropertyBase
{
public:
	typedef InPropType InType::*MemberPtr;

public:
	Property(MemberPtr ptr, const char* name) :
		PropertyBase(name), ptr(ptr) {}

	TString ValueString(const TypedObject* object) const override
	{
		const InType& typed_object = *(InType*)object;
		return StringHelpers::ToString(typed_object.*ptr);
	}

private:
	MemberPtr ptr;
};
