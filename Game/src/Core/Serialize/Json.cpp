
#include <rapidjson/document.h>
#include <rapidjson/pointer.h>
#include <rapidjson/error/en.h>
#include <rapidjson/error/error.h>
#include "Json.h"
#include "Core/OS/File.h"

JsonDocument::~JsonDocument()
{
	if (file_str != nullptr)
		delete[] file_str;
}

void JsonDocument::LoadFile(const char* path)
{
	using namespace rapidjson;
	File::ReadAllDynamic(path, file_str);

	ParseResult pr = json_doc.Parse(file_str);
	if (pr.IsError())
	{
		Error("%s (%d)", GetParseError_En(pr.Code()), pr.Offset());
	}
}

void JsonDocument::Destroy()
{
	json_doc.Clear();
}

JsonValue JsonDocument::GetRootObject()
{
	rapidjson::Value* root_value = &json_doc;
	return JsonValue(root_value);
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
	rapidjson::Value* child_value = rapidjson::Pointer(*path).Get(*value);

	return JsonValue(child_value);
}

JsonValue JsonValue::GetArrayIndex(uint32 index)
{
	if (!IsValid())
		return JsonValue(nullptr);

	TString path = TString::Printf("/%d", index);
	rapidjson::Value* child_value = rapidjson::Pointer(*path).Get(*value);

	return JsonValue(child_value);
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

uint32 JsonValue::ArraySize() const
{
	if (!IsValid())
		return 0;

	return value->Size();
}

bool JsonValue::SerializeChildren(Map<String, JsonValue>& map)
{
	using namespace rapidjson;

	if (!IsValid())
		return false;

	if (!Assert(IsObject()))
		return false;

	// Iterate through all members
	for(Value::MemberIterator it = value->MemberBegin(); it != value->MemberEnd(); ++it)
	{
		Value& val = it->value;
		const char* name = it->name.Get<const char*>();

		map[name] = JsonValue(&val);
	}

	return true;
}
