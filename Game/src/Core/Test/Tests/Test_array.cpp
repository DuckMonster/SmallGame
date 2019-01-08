#include <PCH.h>
#include "../Test.h"

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

	template<typename T>
	bool ArrayTest()
	{
		numConstructedObjects = 0;
		totalConstructed = 0;
		valFail = false;

		{
			ArrayBase<int, T> arr;
			TEST_EXPR(arr.Size() == 0);
			TEST_ERROR(arr[0] = 10);
			TEST_ERROR(arr.RemoveAt(0));

			arr.Add(5);
			TEST_EXPR(arr.Size() == 1);
			TEST_EXPR(arr[0] == 5);
			arr[0] = 10;
			TEST_EXPR(arr[0] == 10);

			arr.Remove(5);
			TEST_EXPR(arr.Size() == 1);
			arr.Remove(10);
			TEST_EXPR(arr.Size() == 0);

			for (int i = 0; i < 10; ++i)
			{
				arr.Add(i);
			}

			TEST_EXPR(arr.Size() == 10);
			arr.Remove(5);
			arr.Remove(7);
			arr.Remove(2);
			arr.Remove(2);
			arr.Remove(30);
			TEST_EXPR(arr.Size() == 7);
			TEST_EXPR(arr.Contains(9));
			TEST_EXPR(!arr.Contains(5));

			{
				uint32 index;
				TEST_EXPR(arr.Find(8, index));
				TEST_EXPR(index == 5);
			}

			TEST_EXPR(arr[5] == 8);
			TEST_ERROR(arr.RemoveAt(7));

			arr.Clear();
			TEST_EXPR(arr.Size() == 0);
			TEST_EXPR(arr.IsEmpty());

			arr.Resize(7, 5);
			for (uint32 i = 0; i < arr.Size(); ++i)
				TEST_EXPR(arr[i] == 5);

			{
				struct TestStr
				{
					TestStr()
					{
						valid = false;
					}
					TestStr(String str)
					{
						valid = true;
					}

					bool valid = false;
				};

				ArrayBase<TestStr, T> strArr;
				strArr.ResizeEmplace(7, "SomeStr");

				bool bValid = true;
				for (uint32 i = 0; i < strArr.Size(); ++i)
					bValid |= strArr[i].valid;

				TEST_EXPR(bValid);
			}
		}

		{
			ArrayBase<ConstructedObject, T> arr;
			TEST_EXPR(numConstructedObjects == 0);

			arr.Add(ConstructedObject());
			TEST_EXPR(numConstructedObjects == 1);
			arr.Remove(ConstructedObject());
			TEST_EXPR(numConstructedObjects == 0);

			for (int i = 0; i < 5; ++i)
			{
				arr.Add(ConstructedObject());
			}

			TEST_EXPR(numConstructedObjects == arr.Size());
			arr.RemoveAt(0);
			TEST_EXPR(numConstructedObjects == arr.Size());
			arr.Remove(ConstructedObject());
			TEST_EXPR(numConstructedObjects == arr.Size());
			arr.Clear();
			TEST_EXPR(numConstructedObjects == 0);

			// Reset total
			totalConstructed = 0;
			arr.Emplace();
			TEST_EXPR(numConstructedObjects == 1);
			TEST_EXPR(arr.Size() == 1);
			TEST_EXPR(totalConstructed == 1);

			arr.Add(ConstructedObject());
			TEST_EXPR(numConstructedObjects == 2);
			TEST_EXPR(arr.Size() == 2);
			TEST_EXPR(totalConstructed == 3);

			arr.Emplace(10);
			TEST_EXPR(numConstructedObjects == 3);
			TEST_EXPR(arr.Size() == 3);
			TEST_EXPR(totalConstructed == 4);

			uint32 index;
			TEST_EXPR(arr.Find(ConstructedObject(10), index));
			TEST_EXPR(index == 2);

			arr[index].val = 20;
			TEST_EXPR(arr.Remove(ConstructedObject(20)));
			TEST_EXPR(arr.Size() == 2);

			arr.Resize(0);
			TEST_EXPR(arr.Size() == 0);
			TEST_EXPR(numConstructedObjects == 0);

			// Add a bunch of them
			arr.Resize(10);
			TEST_EXPR(numConstructedObjects == 10);
			TEST_EXPR(arr.Size() == 10);
			for (int i = 0; i < 10; ++i)
			{
				arr.Emplace(i);
			}
			TEST_EXPR(numConstructedObjects == 20);
			TEST_EXPR(arr.Size() == 20);

			arr.Resize(10);
			TEST_EXPR(numConstructedObjects == 10);
			TEST_EXPR(arr.Size() == 10);
		}

		{
			// Reserve testing
			ArrayBase<int, T> arr;
			arr.Reserve(20);
			arr.Add(5);

			TEST_EXPR(arr[0] == 5);
			TEST_ERROR(arr[1] == 2);
		}

		{
			// Copy testing
			ArrayBase<int, T> arr;
			arr.Add(1);
			arr.Add(2);
			arr.Add(3);

			ArrayBase<int, T> otherArr = arr;
			TEST_EXPR(otherArr.Size() == 3);
			TEST_EXPR(otherArr[0] == 1);
			TEST_EXPR(otherArr[1] == 2);
			TEST_EXPR(otherArr[2] == 3);

			arr[0] = 100;
			TEST_EXPR(arr[0] == 100);
			TEST_EXPR(otherArr[0] == 1);
		}

		{
			// Insert testing
			ArrayBase<int, T> arr;
			arr.Add(0);
			arr.Add(1);
			arr.Add(2);

			TEST_EXPR(arr.Size() == 3);
			arr.Insert(0, 10);
			TEST_EXPR(arr.Size() == 4);
			TEST_EXPR(arr[0] == 10);
			TEST_EXPR(arr[1] == 0);
			TEST_EXPR(arr[2] == 1);
			TEST_EXPR(arr[3] == 2);

			arr.Insert(2, 5);
			TEST_EXPR(arr.Size() == 5);
			TEST_EXPR(arr[0] == 10);
			TEST_EXPR(arr[1] == 0);
			TEST_EXPR(arr[2] == 5);
			TEST_EXPR(arr[3] == 1);
			TEST_EXPR(arr[4] == 2);

			arr.Insert(5, 1);
			TEST_EXPR(arr.Size() == 6);
			TEST_EXPR(arr[0] == 10);
			TEST_EXPR(arr[1] == 0);
			TEST_EXPR(arr[2] == 5);
			TEST_EXPR(arr[3] == 1);
			TEST_EXPR(arr[4] == 2);
			TEST_EXPR(arr[5] == 1);

			TEST_ERROR(arr.Insert(200, 7));
			TEST_ERROR(arr.Insert(200, 10));
		}

		TEST_EXPR(!valFail);

		return true;
	}

	DEFINE_TEST(ArrayTest<HeapAllocator>);
	DEFINE_TEST(ArrayTest<TemporaryAllocator>);
}