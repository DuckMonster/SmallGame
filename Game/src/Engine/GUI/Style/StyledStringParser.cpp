
#include "StyledStringParser.h"

TArray<StyledStringParser::Block> StyledStringParser::Parse(const char* str, const StylePalette& palette)
{
	TArray<StyledStringParser::Block> result;
	Block current_block;
	current_block.style = palette.default_style;

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
				current_block.style = palette.Get(*tag);
			}
			else
			{
				// Empty tag means reverting to default style
				current_block.style = palette.default_style;
			}

			// Check again, because the string could be ended after a tag
			if (*str == '\0')
			{
				break;
			}
		}

		uint32 len = strlen(str);
		uint32 block_length = 0;
		for (; block_length < len; ++block_length)
		{
			// Endline characters counts as the end of a block (but include it in the block)
			if (str[block_length] == '\n')
			{
				block_length++;
				break;
			}

			// Beginning of a tag specifier, end here
			if (str[block_length] == '{')
			{
				break;
			}
		}

		current_block.ptr = str;
		current_block.length = block_length;
		result.Add(current_block);

		str += block_length;
	}

	return result;
}
