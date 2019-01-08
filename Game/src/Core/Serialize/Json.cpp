
#include <rapidjson/document.h>
#include <rapidjson/pointer.h>
#include <rapidjson/error/en.h>
#include <rapidjson/error/error.h>
#include "Json.h"
#include "Core/OS/File.h"

JsonDocument::~JsonDocument()
{
	if (fileStr != nullptr)
		delete[] fileStr;
}

void JsonDocument::LoadFile(const char* path)
{
	using namespace rapidjson;
	File::ReadAllDynamic(path, fileStr);

	ParseResult pr = jsonDoc.Parse(fileStr);
	if (pr.IsError())
	{
		Error("%s (%d)", GetParseError_En(pr.Code()), pr.Offset());
	}
}

void JsonDocument::Destroy()
{
	jsonDoc.Clear();
}

JsonValue JsonDocument::GetRootObject()
{
	rapidjson::Value* rootValue = &jsonDoc;
	return JsonValue(rootValue);
}

TString JsonPath::Parse(const char* path)
{
	TString result;

	// Starts with slash?
	if (path[0] != '/')
	{
		result = "/";
	}

	result += path;

	// Replace '.' with '/'
	for(uint32 i=0; i<result.Length(); ++i)
	{
		if (result[i] == '.')
			result[i] = '/';
	}

	return result;
}

JsonValue JsonValue::GetChild(const char* name)
{
	if (!IsValid())
		return JsonValue(nullptr);

	TString path = JsonPath::Parse(name);
	rapidjson::Value* childValue = rapidjson::Pointer(*path).Get(*value);

	return JsonValue(childValue);
}

JsonValue JsonValue::GetArrayIndex(uint32 index)
{
	if (!IsValid())
		return JsonValue(nullptr);

	TString path = TString::Printf("/%d", index);
	rapidjson::Value* childValue = rapidjson::Pointer(*path).Get(*value);

	return JsonValue(childValue);
}

bool JsonValue::IsArray() const
{
	if (!IsValid())
		return false;

	return value->IsArray();
}

bool JsonValue::IsObject() const
{
	if (!IsValid())
		return false;

	return value->IsObject();
}

uint32 JsonValue::ArrayLen() const
{
	if (!IsValid())
		return 0;

	return value->Size();
}
