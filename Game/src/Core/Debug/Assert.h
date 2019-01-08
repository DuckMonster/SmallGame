#pragma once
#include <stdio.h>
#include <stdlib.h>
#include "TypeDefs.h"
#define Debug_Log(format, ...) printf(format "\n", __VA_ARGS__)
#define Debug_Break() (__debugbreak(), 0)

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

	void Catch() { caughtErrors++; }
	uint32 Count() const { return caughtErrors; }

private:
	uint32 caughtErrors = 0;
};

namespace AssertHelpers
{
	bool AssertLogReturnFalse(const char* expr, const char* file, int line);
	bool ErrorMessage(const char* file, int line, const char* msg, ...);
	bool TryToCatchError();
}

#define Assert(expr) (!!(expr) || AssertHelpers::AssertLogReturnFalse(#expr, __FILE__, __LINE__) || Debug_Break())
#define Error(format, ...) ( AssertHelpers::TryToCatchError() || AssertHelpers::ErrorMessage(__FILE__, __LINE__, format, __VA_ARGS__) || Debug_Break() )
#define Fatal(format, ...) ( AssertHelpers::TryToCatchError() || AssertHelpers::ErrorMessage(__FILE__, __LINE__, format, __VA_ARGS__) || Debug_Break() || (exit(EXIT_FAILURE), 0) )
