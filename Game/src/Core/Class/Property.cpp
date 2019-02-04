
#include "Property.h"
#include "Core/Resource/Resource.h"
#include "Core/Entity/Entity.h"
#include "Core/Component/Component.h"

namespace StringHelpers
{
	TString ToString(const int& value)
	{
		return TString::Printf("%d", value);
	}

	TString ToString(const float& value)
	{
		return TString::Printf("%f", value);
	}

	TString ToString(const Vec2& value)
	{
		return TString::Printf("( %f, %f )", value.x, value.y);
	}

	TString ToString(const Vec3& value)
	{
		return TString::Printf("( %f, %f, %f )", value.x, value.y, value.z);
	}

	TString ToString(const Vec4& value)
	{
		return TString::Printf("( %f, %f, %f, %f )", value.x, value.y, value.z, value.w);
	}

	TString ToString(const Quat& value)
	{
		return TString::Printf("( %f | %f, %f, %f )", value.w, value.x, value.y, value.z);
	}

	TString ToString(const Color& value)
	{
		return TString::Printf("( %f, %f, %f, %f )", value.r, value.g, value.b, value.a);
	}

	TString ToString(const Transform& transform)
	{
		return TString::Printf(
			"p: %s, r: %s, s: %s",
			*ToString(transform.position),
			*ToString(transform.rotation),
			*ToString(transform.scale)
		);
	}

	TString ToString(const Resource* resource)
	{
		return resource->GetPath();
	}

	TString ToString(const Entity* entity)
	{
		return entity->GetName();
	}

	TString ToString(const Component* component)
	{
		return TString::Printf("%s.%s", *component->GetOwner()->GetName(), *component->GetType()->name);
	}

}
