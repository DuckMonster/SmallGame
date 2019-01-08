#include "../Test.h"
#include "Core/Delegate/Event.h"

namespace
{
	uint32 rawCalls = 0;
	void RawDel(int value)
	{
		rawCalls += value;
	}

	class Obj
	{
	public:
		static uint32 staticCalls;
		static void StaticDel(int value)
		{
			staticCalls += value;
		}

		uint32 instanceCalls = 0;
		void InstanceDel(int value)
		{
			instanceCalls += value;
		}
	};
	uint32 Obj::staticCalls = 0;

	bool EventTest()
	{
		int lambdaCalls = 0;
		auto lambda = [&lambdaCalls](int value) { lambdaCalls += value; };

		Event<int> ev;
		ev.AddRaw(RawDel);
		ev.Broadcast(5);

		TEST_EXPR(rawCalls == 5);
		ev.Clear();
		ev.AddRaw(Obj::StaticDel);
		ev.Broadcast(2);
		TEST_EXPR(rawCalls == 5);
		TEST_EXPR(Obj::staticCalls == 2);

		Obj a, b;

		ev.AddObject(&a, &Obj::InstanceDel);
		ev.Broadcast(3);
		TEST_EXPR(Obj::staticCalls == 5);
		TEST_EXPR(a.instanceCalls == 3);
		ev.AddObject(&b, &Obj::InstanceDel);
		ev.Broadcast(1);
		TEST_EXPR(Obj::staticCalls == 6);
		TEST_EXPR(a.instanceCalls == 4);
		TEST_EXPR(b.instanceCalls == 1);
		ev.AddLambda(lambda);
		ev.Broadcast(10);
		TEST_EXPR(Obj::staticCalls == 16);
		TEST_EXPR(a.instanceCalls == 14);
		TEST_EXPR(b.instanceCalls == 11);
		TEST_EXPR(lambdaCalls == 10);

		return true;
	}

	DEFINE_TEST(EventTest);
}
