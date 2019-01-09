#include "../Test.h"
#include "Core/Delegate/Event.h"

namespace
{
	uint32 raw_calls = 0;
	void RawDel(int value)
	{
		raw_calls += value;
	}

	class Obj
	{
	public:
		static uint32 static_calls;
		static void StaticDel(int value)
		{
			static_calls += value;
		}

		uint32 instance_calls = 0;
		void InstanceDel(int value)
		{
			instance_calls += value;
		}
	};
	uint32 Obj::static_calls = 0;

	bool EventTest()
	{
		int lambda_calls = 0;
		auto lambda = [&lambda_calls](int value) { lambda_calls += value; };

		Event<int> ev;
		ev.AddRaw(RawDel);
		ev.Broadcast(5);

		TEST_EXPR(raw_calls == 5);
		ev.Clear();
		ev.AddRaw(Obj::StaticDel);
		ev.Broadcast(2);
		TEST_EXPR(raw_calls == 5);
		TEST_EXPR(Obj::static_calls == 2);

		Obj a, b;

		ev.AddObject(&a, &Obj::InstanceDel);
		ev.Broadcast(3);
		TEST_EXPR(Obj::static_calls == 5);
		TEST_EXPR(a.instance_calls == 3);
		ev.AddObject(&b, &Obj::InstanceDel);
		ev.Broadcast(1);
		TEST_EXPR(Obj::static_calls == 6);
		TEST_EXPR(a.instance_calls == 4);
		TEST_EXPR(b.instance_calls == 1);
		ev.AddLambda(lambda);
		ev.Broadcast(10);
		TEST_EXPR(Obj::static_calls == 16);
		TEST_EXPR(a.instance_calls == 14);
		TEST_EXPR(b.instance_calls == 11);
		TEST_EXPR(lambda_calls == 10);

		return true;
	}

	DEFINE_TEST(EventTest);
}
