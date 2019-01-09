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
			Type* type_a = Type::Get<TestTypeA>();
			TEST_EXPR(type_a != nullptr);

			TestTypeA* obj_a = NewObject<TestTypeA>(type_a);
			TEST_EXPR(obj_a != nullptr);

			Type* other_type = obj_a->GetType();
			TEST_EXPR(type_a == other_type);

			TestTypeB* obj_b = NewObject<TestTypeB>();
			TEST_EXPR(obj_b != nullptr);

			other_type = Type::Get("TestTypeA");
			TEST_EXPR(type_a == other_type);

			Type* c_type = Type::Get<TestTypeC>();
			TEST_EXPR(c_type == nullptr);
			TEST_ERROR(NewObject<TestTypeC>());
		}

		{
			TestTypeA* obj_a = NewObject<TestTypeA>();

			Type* a_type = obj_a->GetType();
			TEST_EXPR(a_type->properties.Size() == 1);

			PropertyBase* a_prop = a_type->properties[0];
			TEST_EXPR(a_prop->name == "a");
			TEST_EXPR(a_prop->ValueString(obj_a) == "10");

			obj_a->a = 200;
			TEST_EXPR(a_prop->ValueString(obj_a) == "200");
		}

		return true;
	}

	DEFINE_TEST(TypeTest);
}