
#include "Assert.h"
#include <windows.h>
#include <stdarg.h>
#include "Core/OS/Path.h"

ErrorScope* ErrorScope::active = nullptr;

bool AssertHelpers::AssertLogReturnFalse(const char* expr, const char* file, int line)
{
	TString file_str = Path::GetFile(file);
	file = *file_str;

	char* buffer = talloc(1024);
	sprintf_s(buffer, 1024, "ASSERT FAILED (%s:%d)\n%s", file, line, expr);

	Debug_Log("ASSERT FAILED (%s (%d))\n%s\n", file, line, expr);
	MessageBox(NULL, buffer, "Assert Failed", 0);
	return false;
}

bool AssertHelpers::ErrorMessage(const char* file, int line, const char* msg, ...)
{
	TString file_str = Path::GetFile(file);
	file = *file_str;

	// Get the message
	va_list list;
	va_start(list, msg);

	// Get length of formatted message
	int len = vsnprintf(nullptr, 0, msg, list);

	// Then get it
	char* msg_buffer = talloc(len + 1);
	vsprintf_s(msg_buffer, len + 1, msg, list);

	va_end(list);

	// Write the prompt message
	String prompt_msg = String::Printf("Error (%s (%d))\n%s", file, line, msg_buffer);

	Debug_Log("Error (%s (%d))\n%s", file, line, msg_buffer);
	MessageBox(NULL, *prompt_msg, "Error", MB_OK);
	return false;
}

bool AssertHelpers::TryToCatchError()
{
	if (ErrorScope::active == nullptr)
		return false;

	ErrorScope::active->Catch();
	return true;
}
