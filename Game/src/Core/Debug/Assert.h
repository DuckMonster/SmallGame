#pragma once
#include <stdio.h>
#include <stdlib.h>
#include "TypeDefs.h"
#define Debug_Log(format, ...) LogHelpers::DebugLog(format "\n", __VA_ARGS__)
#define Debug_Break() (AssertHelpers::IsDebuggerAttached() && (__debugbreak(), 0))

struct ErrorScope
{
public:
	static ErrorScope* active;
	ErrorScope()
	{
		active = this;
	}
	~ErrorScope()
	{
		active = nullptr;
	}

	void Catch() { cought_errors++; }
	uint32 Count() const { return cought_errors; }

private:
	uint32 cought_errors = 0;
};

namespace AssertHelpers
{
	bool AssertLogReturnFalse(const char* expr, const char* file, int line);
	bool ErrorMessage(const char* file, int line, const char* msg, ...);
	bool TryToCatchError();
	bool IsDebuggerAttached();
}

namespace LogHelpers
{
	void DebugLog(const char* format, ...);
}

#define Assert(expr) (!!(expr) || AssertHelpers::AssertLogReturnFalse(#expr, __FILE__, __LINE__) || Debug_Break())
#define Error(format, ...) ( AssertHelpers::TryToCatchError() || AssertHelpers::ErrorMessage(__FILE__, __LINE__, format, __VA_ARGS__) || Debug_Break() )
#define Fatal(format, ...) ( AssertHelpers::TryToCatchError() || AssertHelpers::ErrorMessage(__FILE__, __LINE__, format, __VA_ARGS__) || Debug_Break() || (exit(EXIT_FAILURE), 0) )
