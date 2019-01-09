
#include "Path.h"
#define IS_SLASH(c) (c == '\\' || c == '/')
TString Path::Clean(const char* path)
{
	std::string result = path;
	bool prev_slash = true;

	for (int i = 0; i < result.size(); ++i)
	{
		// Remove groups of slashes
		if (prev_slash && IS_SLASH(result[i]))
		{
			result.erase(i, 1);
			i--;
			continue;
		}

		prev_slash = IS_SLASH(result[i]);

		if (result[i] == '\\')
			result[i] = '/';
	}

	return result;
}

TString Path::Join(const char* first, const char* second)
{
	TString first_str = Clean(first);
	TString second_str = Clean(second);

	if (first_str[first_str.Length() - 1] != '/')
		first_str += "/";

	return first_str + second_str;
}

TString Path::GetFile(const char* path)
{
	int last_slash = 0;
	int i = 0;
	while (path[i] != '\0')
	{
		if (IS_SLASH(path[i]))
			last_slash = i;

		i++;
	}

	return TString(path + last_slash + 1);
}
