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

	static int someFuncCalls = 0;
	void SomeFunc()
	{
		someFuncCalls++;
	}
	void SomeFuncTwice()
	{
		someFuncCalls += 2;
	}

	class DelTestClass
	{
	public:
		static int staticValue;
		static int StaticFunc(int value)
		{
			staticValue += value;
			return staticValue;
		}

		int instanceValue = 0;
		int InstanceFunc(int value)
		{
			instanceValue += value;
			return instanceValue;
		}
	};
	int DelTestClass::staticValue = 0;

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
			double delResult = 0.0;

			{
				Delegate<double, int, double> del;

				del.BindRaw(AddFunc);
				TEST_EXPR(del(5, 20.0) == 5 + 20.0);
				del.BindRaw(MinusFunc);
				TEST_EXPR(del(10, 2.5) == 10 - 2.5);

				delResult = del(10, 2.5);
				other = del;
			}

			TEST_EXPR(other(10, 2.5) == delResult);
		}

		{
			DelTestClass a;
			DelTestClass b;
			Delegate<int, int> del;
			del.BindObject(&a, &DelTestClass::InstanceFunc);

			TEST_EXPR(del(5) == 5);
			TEST_EXPR(del(2) == 7);
			TEST_EXPR(a.instanceValue == 7);

			del.BindObject(&b, &DelTestClass::InstanceFunc);
			TEST_EXPR(del(3) == 3);
			TEST_EXPR(del(1) == 4);
			TEST_EXPR(b.instanceValue == 4);
			TEST_EXPR(a.instanceValue == 7);
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
