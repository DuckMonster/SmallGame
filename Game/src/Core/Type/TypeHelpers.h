#pragma once
#include "Property.h"
#include "Type.h"

template<typename T>
class TypeRegisterHelper
{
public:
	TypeRegisterHelper(const char* name) 
	{
		Reflection::RegisterType<T>(name);
	}
};

template<typename InType, typename InPropType>
class PropertyRegisterHelper
{
public:
	PropertyRegisterHelper(typename Property<InType, InPropType>::MemberPtr ptr, const char* name)
	{
		Property<InType, InPropType>* prop = new Property<InType, InPropType>(ptr, name);
		Type* type = Type::Get<InType>();
		type->properties.Add(prop);
	}
};

template<typename InType, typename InPropType>
PropertyRegisterHelper<InType, InPropType> MakePropHelper(InPropType InType::*ptr, const char* name)
{
	return PropertyRegisterHelper<InType, InPropType>(ptr, name);
}

#define REGISTER_TYPE(cls) TypeRegisterHelper<cls> _REGISTER_ ## cls(#cls)
#define REGISTER_PROP(cls, prop) auto _REGISTER_ ## cls ## __ ## prop = MakePropHelper(&cls::prop, #prop)