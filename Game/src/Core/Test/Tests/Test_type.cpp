#include <PCH.h>
#include "../Test.h"
#include "Core/Class/Type.h"
#include "Core/Class/TypedObject.h"
#include "Core/Class/TypeHelpers.h"

namespace
{
	class TestTypeA : public TypedObject
	{
	public:
		int a = 10;
	};

	class TestTypeB : public TypedObject
	{
	public:
		int b = 20;
	};

	class TestTypeC : public TypedObject
	{
	public:
		int c = 30;
	};

	REGISTER_TYPE(TestTypeA);
	REGISTER_TYPE(TestTypeB);
	REGISTER_PROP(TestTypeA, a);
	REGISTER_PROP(TestTypeB, b);

	bool TypeTest()
	{
		{
			Type* typeA = Type::Get<TestTypeA>();
			TEST_EXPR(typeA != nullptr);

			TestTypeA* objA = NewObject<TestTypeA>(typeA);
			TEST_EXPR(objA != nullptr);

			Type* otherType = objA->GetType();
			TEST_EXPR(typeA == otherType);

			TestTypeB* objB = NewObject<TestTypeB>();
			TEST_EXPR(objB != nullptr);

			otherType = Type::Get("TestTypeA");
			TEST_EXPR(typeA == otherType);

			Type* cType = Type::Get<TestTypeC>();
			TEST_EXPR(cType == nullptr);
			TEST_ERROR(NewObject<TestTypeC>());
		}

		{
			TestTypeA* objA = NewObject<TestTypeA>();

			Type* aType = objA->GetType();
			TEST_EXPR(aType->properties.Size() == 1);

			PropertyBase* aProp = aType->properties[0];
			TEST_EXPR(aProp->name == "a");
			TEST_EXPR(aProp->ValueString(objA) == "10");

			objA->a = 200;
			TEST_EXPR(aProp->ValueString(objA) == "200");
		}

		return true;
	}

	DEFINE_TEST(TypeTest);
}