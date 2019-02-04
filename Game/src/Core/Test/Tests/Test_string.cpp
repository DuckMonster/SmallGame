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
			str = "one Two Three";
			StringBase<T> sub_str = str.SubStr(0, 3);
			TEST_EXPR(sub_str == "one");
			sub_str = str.SubStr(4, 3);
			TEST_EXPR(sub_str == "Two");
			sub_str = str.SubStr(8, 5);
			TEST_EXPR(sub_str == "Three");
			TEST_ERROR(str.SubStr(0, 20));

			sub_str = str.SubStr(12, 1);
			TEST_EXPR(sub_str == "e");

			TEST_NOERROR(str.SubStr(12, 0));
			TEST_ERROR(str.SubStr(13, 0));
		}

		{
			// Find
			StringBase<T> str = "Hello World!";
			int32 find_index = str.Find("Hello");
			TEST_EXPR(str.Find("Hello") == 0);
			TEST_EXPR(str.Find("Dang") == -1);
			TEST_EXPR(str.Find("World") == 6);
			TEST_EXPR(str.Find("World", 6) == 6);
			TEST_EXPR(str.Find("World", 7) == -1);
			TEST_EXPR(str.Find("Hello", 1) == -1);
			TEST_EXPR(str.Find("") == 0);
			TEST_ERROR(str.Find(nullptr));
		}

		{
			// Replace
			StringBase<T> str = "Hello World";
			TEST_EXPR(str.Replace("Hello", "Bello") == "Bello World");
			TEST_EXPR(str.Replace("Hello", "Yo") == "Yo World");
			TEST_EXPR(str.Replace("World", "Mama") == "Hello Mama");
			TEST_EXPR(str.Replace("Asd", "Boopidoopi") == "Hello World");

			str = "Yo Yo Yo Bo Yo";

			TEST_EXPR(str.Replace("Yo", "To") == "To To To Bo To");

			str = "This is a {VAR} sentence with {VAR} flowers";
			TEST_EXPR(str.Replace("{VAR}", "nice") == "This is a nice sentence with nice flowers");
			TEST_EXPR(str.Replace("{VAR}", "horrible") == "This is a horrible sentence with horrible flowers");
		}

		return true;
	}

	DEFINE_TEST(StringTest<HeapAllocator>);
	DEFINE_TEST(StringTest<TemporaryAllocator>);
}