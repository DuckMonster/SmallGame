#pragma once
class Path
{
public:
	static TString Clean(const char* path);
	static TString Join(const char* first, const char* second);
	static TString GetFile(const char* path);
};
