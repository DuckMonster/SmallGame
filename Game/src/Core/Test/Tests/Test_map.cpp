#include <PCH.h>
#include "../Test.h"
#include "Core/Standard/Map.h"

namespace
{
	uint32 totalConstructed = 0;
	uint32 numConstructedObjects = 0;
	bool valFail = false;

	struct ConstructedObject
	{
	public:
		ConstructedObject()
		{
			if (val != 0)
				valFail = true;

			numConstructedObjects++;
			totalConstructed++;
		}
		ConstructedObject(const ConstructedObject& other) : ConstructedObject() {}
		ConstructedObject(uint32 val) : val(val)
		{
			numConstructedObjects++;
			totalConstructed++;
		}
		~ConstructedObject()
		{
			numConstructedObjects--;
		}

		bool operator==(const ConstructedObject& other) const
		{
			return val == other.val;
		}
		bool operator!=(const ConstructedObject& other) const
		{
			return val != other.val;
		}
		int val = 0;
	};

	template<typename AllocType>
	bool MapTest()
	{
		numConstructedObjects = 0;
		totalConstructed = 0;
		valFail = false;

		{
			MapBase<int, String, AllocType> map;
			map.Add(5, "Hello");

			TEST_EXPR(map.Contains(5));
			TEST_EXPR(map[5] == "Hello");
			TEST_EXPR(!map.Contains(10));

			{
				String* str = map.Find(5);
				TEST_EXPR(str != nullptr);
				TEST_EXPR(*str == "Hello");
				*str = "Dummy";
				TEST_EXPR(map[5] == "Dummy");
				map[5] = "Hello";
				TEST_EXPR(*str == "Hello");

				str = map.Find(2);
				TEST_EXPR(str == nullptr);
			}

			map[5] = "Yo!";
			TEST_EXPR(map[5] == "Yo!");
			map[10] = "Dope";
			TEST_EXPR(map[10] == "Dope");

			map.Remove(5);
			TEST_EXPR(!map.Contains(5));
			map.Clear();
			TEST_EXPR(!map.Contains(10));
		}

		{
			MapBase<StringBase<AllocType>, ConstructedObject, AllocType> map;

			map.Add("Hello", ConstructedObject());
			map.Add("Dude", ConstructedObject(5));

			TEST_EXPR(map.Contains("Hello"));
			TEST_EXPR(map.Contains("Dude"));
			TEST_EXPR(map.Contains(TString("Hello")));
			TEST_EXPR(map.Contains(TString("Dude")));
			TEST_ERROR(map.Add(TString("Dude"), ConstructedObject(10)));

			TEST_EXPR(numConstructedObjects == 2);

			TEST_EXPR(map["Dude"].val == 5);

			TEST_EXPR(map.Remove("Dude"));
			TEST_EXPR(!map.Remove("Fake"));
			TEST_EXPR(!map.Contains("Dude"));

			TEST_EXPR(map.Remove("Hello"));
			TEST_EXPR(numConstructedObjects == 0);
		}

		{
			MapBase<ConstructedObject, StringBase<AllocType>, AllocType> map;
			ConstructedObject first, second;
			first.val = 5;
			second.val = 10;

			map.Add(first, "First");
			map.Add(second, "Second");
			TEST_ERROR(map.Add(first, "First"));
			TEST_EXPR(map[first] == "First");
			TEST_EXPR(map[second] == "Second");
			map[first] = map[second];
			TEST_EXPR(map[first] == map[second]);

			TEST_EXPR(numConstructedObjects == 4);

			TEST_EXPR(map.Remove(second));
			TEST_EXPR(!map.Contains(second));
			TEST_EXPR(map.Contains(first));
			TEST_EXPR(map[first] == "Second");

			second = first;

			TEST_EXPR(map.Remove(second));
			TEST_EXPR(!map.Contains(first));

			TEST_EXPR(numConstructedObjects == 2);
		}

		{
			// Copy constructor, etc.
			MapBase<uint32, String, AllocType> map1;
			map1[5] = "5";
			map1[10] = "10";
			map1[0] = "0";

			MapBase<uint32, String, AllocType> map2 = map1;
			map1.Clear();
			TEST_EXPR(map2[5] == "5");
			TEST_EXPR(map2[10] == "10");
			TEST_EXPR(map2[0] == "0");
			map1[5] = "50";
			TEST_EXPR(map1[5] == "50");
			TEST_EXPR(map2[5] == "5");
		}

		{
			// Allocator conversions
			Map<uint32, String> map1;
			map1[5] = "5";
			map1[10] = "10";
			map1[0] = "0";

			TMap<uint32, String> map2 = map1;
			map1.Clear();
			TEST_EXPR(map2[5] == "5");
			TEST_EXPR(map2[10] == "10");
			TEST_EXPR(map2[0] == "0");
			map1[5] = "50";
			TEST_EXPR(map1[5] == "50");
			TEST_EXPR(map2[5] == "5");
		}

		TEST_EXPR(!valFail);

		return true;
	}

	DEFINE_TEST(MapTest<HeapAllocator>);
	DEFINE_TEST(MapTest<TemporaryAllocator>);
}