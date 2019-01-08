
#include "StyledStringParser.h"

TArray<StyledStringParser::Block> StyledStringParser::Parse(const char* str, const StylePalette& palette)
{
	TArray<StyledStringParser::Block> result;
	Block currentBlock;
	currentBlock.style = palette.defaultStyle;

	// Main loop
	while (*str)
	{
		// Check if this block starts with a style tag
		while (*str == '{')
		{
			str++;

			// Extract tag by finding the closing bracket
			TString tag;
			uint32 len = strlen(str);
			for(uint32 i=0; i<len; ++i)
			{
				if (str[i] == '}')
				{
					tag.Set(str, i);
					str += i + 1;
					break;
				}
			}

			if (!tag.IsEmpty())
			{
				// Get the style from the tag
				currentBlock.style = palette.Get(*tag);
			}
			else
			{
				// Empty tag means reverting to default style
				currentBlock.style = palette.defaultStyle;
			}

			// Check again, because the string could be ended after a tag
			if (*str == '\0')
			{
				break;
			}
		}

		uint32 len = strlen(str);
		uint32 blockLength = 0;
		for (; blockLength < len; ++blockLength)
		{
			// Endline characters counts as the end of a block (but include it in the block)
			if (str[blockLength] == '\n')
			{
				blockLength++;
				break;
			}

			// Beginning of a tag specifier, end here
			if (str[blockLength] == '{')
			{
				break;
			}
		}

		currentBlock.ptr = str;
		currentBlock.length = blockLength;
		result.Add(currentBlock);

		str += blockLength;
	}

	return result;
}
