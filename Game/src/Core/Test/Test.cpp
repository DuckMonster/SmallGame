
#include "Test.h"

void Test::Run()
{
	int totalTests = 0;
	int failedTests = 0;

	for (uint32 i = 0; i < GetDefinitions().Size(); ++i)
	{
		Test::TestDefinition& def = GetDefinitions()[i];

		Debug_Log("### %s ###", *def.file);
		bool result = def.callback();
		Debug_Log("### %s ###\n", result ? "SUCCESS" : "FAIL");

		totalTests++;
		if (!result)
		{
			failedTests++;
			break;
		}
	}

	Debug_Log("### TESTING COMPLETE ###");
	Debug_Log("%s (%d / %d)", failedTests == 0 ? "SUCCESS" : "FAIL", totalTests - failedTests, totalTests);
	system("pause");
}

Array<Test::TestDefinition>& Test::GetDefinitions()
{
	static Array<Test::TestDefinition> DefinitionList;
	return DefinitionList;
}

