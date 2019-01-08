#include <PCH.h>
#include "../Test.h"

namespace
{
	template<typename T>
	bool StringTest()
	{
		{
			StringBase<T> str;
			TEST_EXPR(str.Length() == 0);
			TEST_EXPR(str.IsEmpty());
			str = "MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM";
			TEST_EXPR(str.Length() == 32);
			str = "";
			TEST_EXPR(str.Length() == 0);
			TEST_EXPR(str.IsEmpty());

			str += "Hello";
			TEST_EXPR(str == "Hello");
			TEST_EXPR(str != "Hellon");
			TEST_EXPR(str == String("Hello"));
			TEST_EXPR(str != String("Hellon"));
			TEST_EXPR(str == TString("Hel") + "lo");
			TEST_EXPR(str + " there!" == TString("Hel") + "lo" + " there!");

			TEST_EXPR((String("Damn") + TString("You") + String("Sir")).Length() == 10);
			TEST_EXPR((TString("Dude") + " whats up?") == "Dude whats up?");
		}

		{
			StringBase<T> str;
			str = "One Two Three";
			StringBase<T> subStr = str.SubStr(0, 3);
			TEST_EXPR(subStr == "One");
			subStr = str.SubStr(4, 3);
			TEST_EXPR(subStr == "Two");
			subStr = str.SubStr(8, 5);
			TEST_EXPR(subStr == "Three");
			TEST_ERROR(str.SubStr(0, 20));

			subStr = str.SubStr(12, 1);
			TEST_EXPR(subStr == "e");

			TEST_NOERROR(str.SubStr(12, 0));
			TEST_ERROR(str.SubStr(13, 0));
		}

		return true;
	}

	DEFINE_TEST(StringTest<HeapAllocator>);
	DEFINE_TEST(StringTest<TemporaryAllocator>);
}