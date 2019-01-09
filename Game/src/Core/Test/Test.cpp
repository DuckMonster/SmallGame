
#include "Test.h"

void Test::Run()
{
	int total_tests = 0;
	int failed_tests = 0;

	for (uint32 i = 0; i < GetDefinitions().Size(); ++i)
	{
		Test::TestDefinition& def = GetDefinitions()[i];

		Debug_Log("### %s ###", *def.file);
		bool result = def.callback();
		Debug_Log("### %s ###\n", result ? "SUCCESS" : "FAIL");

		total_tests++;
		if (!result)
		{
			failed_tests++;
			break;
		}
	}

	Debug_Log("### TESTING COMPLETE ###");
	Debug_Log("%s (%d / %d)", failed_tests == 0 ? "SUCCESS" : "FAIL", total_tests - failed_tests, total_tests);
	system("pause");
}

Array<Test::TestDefinition>& Test::GetDefinitions()
{
	static Array<Test::TestDefinition> DefinitionList;
	return DefinitionList;
}

