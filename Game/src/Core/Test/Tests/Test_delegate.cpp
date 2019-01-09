#include "../Test.h"
#include "Core/Delegate/Delegate.h"

namespace
{
	double AddFunc(int i, double d)
	{
		return i + d;
	}

	double MinusFunc(int i, double d)
	{
		return i - d;
	}

	static int some_func_calls = 0;
	void SomeFunc()
	{
		some_func_calls++;
	}
	void SomeFuncTwice()
	{
		some_func_calls += 2;
	}

	class DelTestClass
	{
	public:
		static int static_value;
		static int StaticFunc(int value)
		{
			static_value += value;
			return static_value;
		}

		int instance_value = 0;
		int InstanceFunc(int value)
		{
			instance_value += value;
			return instance_value;
		}
	};
	int DelTestClass::static_value = 0;

	int CallIntDelegate(Delegate<int, int, int> d, int a, int b)
	{
		return d(a, b);
	}
	int iAdd(int a, int b) { return a + b; }
	int iSubtract(int a, int b) { return a - b; }

	bool DelegateTest()
	{
		{
			Delegate<double, int, double> other;
			double del_result = 0.0;

			{
				Delegate<double, int, double> del;

				del.BindRaw(AddFunc);
				TEST_EXPR(del(5, 20.0) == 5 + 20.0);
				del.BindRaw(MinusFunc);
				TEST_EXPR(del(10, 2.5) == 10 - 2.5);

				del_result = del(10, 2.5);
				other = del;
			}

			TEST_EXPR(other(10, 2.5) == del_result);
		}

		{
			DelTestClass a;
			DelTestClass b;
			Delegate<int, int> del;
			del.BindObject(&a, &DelTestClass::InstanceFunc);

			TEST_EXPR(del(5) == 5);
			TEST_EXPR(del(2) == 7);
			TEST_EXPR(a.instance_value == 7);

			del.BindObject(&b, &DelTestClass::InstanceFunc);
			TEST_EXPR(del(3) == 3);
			TEST_EXPR(del(1) == 4);
			TEST_EXPR(b.instance_value == 4);
			TEST_EXPR(a.instance_value == 7);
		}

		{
			int value = 0;
			Delegate<void, int> del;
			del.BindLambda([&value](int v) {
				value += v;
			});

			del(10);
			TEST_EXPR(value == 10);
			del(5);
			TEST_EXPR(value == 15);

			del.BindLambda([&value](int v) {
				value -= v;
			});

			del(2);
			TEST_EXPR(value == 13);
			del.Clear();
			del(15);
			TEST_EXPR(value == 13);
		}

		{
			typedef Delegate<int, int, int> iDel;
			iDel d;
			d.BindRaw(iAdd);

			TEST_EXPR(CallIntDelegate(d, 5, 5) == 10);
			TEST_EXPR(CallIntDelegate(iDel::Raw(iSubtract), 5, 2) == 3);

			int a = 5, b = 10;
			TEST_EXPR(CallIntDelegate(iDel::Lambda([a, b](int, int) -> int {
				return a + b;
			}), 20, 2) == 15);
		}

		return true;
	}

	DEFINE_TEST(DelegateTest);
}
