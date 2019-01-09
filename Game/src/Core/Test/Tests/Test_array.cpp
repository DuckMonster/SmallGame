#include <PCH.h>
#include "../Test.h"

namespace
{
	uint32 total_constructed = 0;
	uint32 num_constructed_objects = 0;
	bool val_fail = false;

	struct ConstructedObject
	{
	public:
		ConstructedObject()
		{
			if (val != 0)
				val_fail = true;

			num_constructed_objects++;
			total_constructed++;
		}
		ConstructedObject(const ConstructedObject& other) : ConstructedObject() {}
		ConstructedObject(uint32 val) : val(val)
		{
			num_constructed_objects++;
			total_constructed++;
		}
		~ConstructedObject()
		{
			num_constructed_objects--;
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
		num_constructed_objects = 0;
		total_constructed = 0;
		val_fail = false;

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

				ArrayBase<TestStr, T> str_arr;
				str_arr.ResizeEmplace(7, "SomeStr");

				bool valid = true;
				for (uint32 i = 0; i < str_arr.Size(); ++i)
					valid |= str_arr[i].valid;

				TEST_EXPR(valid);
			}
		}

		{
			ArrayBase<ConstructedObject, T> arr;
			TEST_EXPR(num_constructed_objects == 0);

			arr.Add(ConstructedObject());
			TEST_EXPR(num_constructed_objects == 1);
			arr.Remove(ConstructedObject());
			TEST_EXPR(num_constructed_objects == 0);

			for (int i = 0; i < 5; ++i)
			{
				arr.Add(ConstructedObject());
			}

			TEST_EXPR(num_constructed_objects == arr.Size());
			arr.RemoveAt(0);
			TEST_EXPR(num_constructed_objects == arr.Size());
			arr.Remove(ConstructedObject());
			TEST_EXPR(num_constructed_objects == arr.Size());
			arr.Clear();
			TEST_EXPR(num_constructed_objects == 0);

			// Reset total
			total_constructed = 0;
			arr.Emplace();
			TEST_EXPR(num_constructed_objects == 1);
			TEST_EXPR(arr.Size() == 1);
			TEST_EXPR(total_constructed == 1);

			arr.Add(ConstructedObject());
			TEST_EXPR(num_constructed_objects == 2);
			TEST_EXPR(arr.Size() == 2);
			TEST_EXPR(total_constructed == 3);

			arr.Emplace(10);
			TEST_EXPR(num_constructed_objects == 3);
			TEST_EXPR(arr.Size() == 3);
			TEST_EXPR(total_constructed == 4);

			uint32 index;
			TEST_EXPR(arr.Find(ConstructedObject(10), index));
			TEST_EXPR(index == 2);

			arr[index].val = 20;
			TEST_EXPR(arr.Remove(ConstructedObject(20)));
			TEST_EXPR(arr.Size() == 2);

			arr.Resize(0);
			TEST_EXPR(arr.Size() == 0);
			TEST_EXPR(num_constructed_objects == 0);

			// Add a bunch of them
			arr.Resize(10);
			TEST_EXPR(num_constructed_objects == 10);
			TEST_EXPR(arr.Size() == 10);
			for (int i = 0; i < 10; ++i)
			{
				arr.Emplace(i);
			}
			TEST_EXPR(num_constructed_objects == 20);
			TEST_EXPR(arr.Size() == 20);

			arr.Resize(10);
			TEST_EXPR(num_constructed_objects == 10);
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

			ArrayBase<int, T> other_arr = arr;
			TEST_EXPR(other_arr.Size() == 3);
			TEST_EXPR(other_arr[0] == 1);
			TEST_EXPR(other_arr[1] == 2);
			TEST_EXPR(other_arr[2] == 3);

			arr[0] = 100;
			TEST_EXPR(arr[0] == 100);
			TEST_EXPR(other_arr[0] == 1);
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

		TEST_EXPR(!val_fail);

		return true;
	}

	DEFINE_TEST(ArrayTest<HeapAllocator>);
	DEFINE_TEST(ArrayTest<TemporaryAllocator>);
}