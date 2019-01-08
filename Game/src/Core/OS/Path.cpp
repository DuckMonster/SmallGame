
#include "Path.h"
#define IS_SLASH(c) (c == '\\' || c == '/')
TString Path::Clean(const char* path)
{
	std::string result = path;
	bool prevSlash = true;

	for (int i = 0; i < result.size(); ++i)
	{
		// Remove groups of slashes
		if (prevSlash && IS_SLASH(result[i]))
		{
			result.erase(i, 1);
			i--;
			continue;
		}

		prevSlash = IS_SLASH(result[i]);

		if (result[i] == '\\')
			result[i] = '/';
	}

	return result;
}

TString Path::Join(const char* first, const char* second)
{
	TString firstStr = Clean(first);
	TString secondStr = Clean(second);

	if (firstStr[firstStr.Length() - 1] != '/')
		firstStr += "/";

	return firstStr + secondStr;
}

TString Path::GetFile(const char* path)
{
	int lastSlash = 0;
	int i = 0;
	while (path[i] != '\0')
	{
		if (IS_SLASH(path[i]))
			lastSlash = i;

		i++;
	}

	return TString(path + lastSlash + 1);
}
