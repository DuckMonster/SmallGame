#pragma once
#define TEST_EXPR(expr) do {\
bool result = !!(expr);\
Debug_Log("[%d]\t%s%s", __LINE__, #expr, result ? "" : " (FAIL)");\
if (!result)\
{\
	Debug_Break();\
	return false;\
}\
} while(0)

#define TEST_ERROR(expr) do {\
ErrorScope scope;\
expr;\
bool result = scope.Count() > 0;\
Debug_Log("[%d]\tERROR( %s )%s", __LINE__, #expr, result ? "" : " (FAIL)");\
if (!result)\
	return false;\
} while(0)

#define TEST_NOERROR(expr) do {\
ErrorScope scope;\
expr;\
bool result = scope.Count() == 0;\
Debug_Log("[%d]\tERROR( %s )%s", __LINE__, #expr, result ? "" : " (FAIL)");\
if (!result)\
	return false;\
} while(0)

#define DEFINE_TEST(function) \
namespace {\
Test::TestDefinition CONCAT(TEST_, __LINE__)(#function, &function);\
}

class Test
{
public:
	/** Struct used to define a test function, declare these in C++ files to add a test
	*******************************************************************************/
	struct TestDefinition
	{
		typedef bool (*TestFunction)();
		TestDefinition() {}
		TestDefinition(const char* file, TestFunction callback) : file(file), callback(callback)
		{
			Test::GetDefinitions().Emplace(*this);
		}

		String file;
		TestFunction callback;
	};

public:
	static void Run();

protected:
	// Static function to ensure the array variable is actually created (since we cant ensure the order)
	static Array<TestDefinition>& GetDefinitions();
};