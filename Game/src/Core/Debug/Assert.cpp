
#include "Assert.h"
#include <windows.h>
#include <stdarg.h>
#include "Core/OS/Path.h"

ErrorScope* ErrorScope::active = nullptr;

bool AssertHelpers::AssertLogReturnFalse(const char* expr, const char* file, int line)
{
	TString fileStr = Path::GetFile(file);
	file = *fileStr;

	char* buffer = talloc(1024);
	sprintf_s(buffer, 1024, "ASSERT FAILED (%s:%d)\n%s", file, line, expr);

	Debug_Log("ASSERT FAILED (%s (%d))\n%s\n", file, line, expr);
	MessageBox(NULL, buffer, "Assert Failed", 0);
	return false;
}

bool AssertHelpers::ErrorMessage(const char* file, int line, const char* msg, ...)
{
	TString fileStr = Path::GetFile(file);
	file = *fileStr;

	// Get the message
	va_list list;
	va_start(list, msg);

	// Get length of formatted message
	int len = vsnprintf(nullptr, 0, msg, list);

	// Then get it
	char* msgBuffer = talloc(len + 1);
	vsprintf_s(msgBuffer, len + 1, msg, list);

	va_end(list);

	// Write the prompt message
	String promptMsg = String::Printf("Error (%s (%d))\n%s", file, line, msgBuffer);

	Debug_Log("Error (%s (%d))\n%s", file, line, msgBuffer);
	MessageBox(NULL, *promptMsg, "Error", MB_OK);
	return false;
}

bool AssertHelpers::TryToCatchError()
{
	if (ErrorScope::active == nullptr)
		return false;

	ErrorScope::active->Catch();
	return true;
}
