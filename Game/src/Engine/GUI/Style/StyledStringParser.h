#pragma once
#include "Style.h"

class StyledStringParser
{
public:
	struct Block
	{
		Block() {}
		Block(const char* ptr, uint32 length) :
			ptr(ptr), length(length) {}

		const char* ptr = nullptr;
		uint32 length = 0;
		Style style;
	};

	// Parses a string, and returns the string divided into styled blocks specified by tags
	static TArray<Block> Parse(const char* str, const StylePalette& palette);
};