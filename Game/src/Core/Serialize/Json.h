#pragma once
#include <rapidjson/document.h>
#include <rapidjson/pointer.h>

class JsonValue;

class JsonPath
{
public:
	static TString Parse(const char* path);
};

class JsonDocument
{
public:
	~JsonDocument();
	void LoadFile(const char* path);
	void Destroy();

	JsonValue GetRootObject();

private:
	char* file_str = nullptr;
	rapidjson::Document json_doc;
};

class JsonValue
{
public:
	JsonValue(rapidjson::Value* value) : value(value) {}

	template<typename T>
	bool Serialize(T& value);
	template<typename T>
	bool Serialize(const char* name, T& var) { return GetChild(name).Serialize<T>(var); }

	// Template specializations
	template<>
	bool Serialize<Color>(Color& var) { return SerializeArray<float>((float*)&var, 4); }
	template<>
	bool Serialize<Vec2>(Vec2& var) { return SerializeArray<float>((float*)&var, 2); }
	template<>
	bool Serialize<Vec3>(Vec3& var) { return SerializeArray<float>((float*)&var, 3); }
	template<>
	bool Serialize<Vec4>(Vec4& var) { return SerializeArray<float>((float*)&var, 4); }
	template<typename InAlloc>
	bool Serialize(StringBase<InAlloc>& var)
	{
		const char* c_str = nullptr;
		if (!Serialize(c_str))
			return false;

		var = c_str;
		return true;
	}

	template<typename T, typename TAlloc>
	bool SerializeArray(ArrayBase<T, TAlloc>& arr);
	template<typename T, typename TAlloc>
	bool SerializeArray(const char* name, ArrayBase<T, TAlloc>& arr) { return GetChild(name).SerializeArray<T>(arr); }
	template<typename T>
	bool SerializeArray(T* arr, uint32 count);
	template<typename T>
	bool SerializeArray(const char* name, T* arr, uint32 count) { return GetChild(name).SerializeArray<T>(arr, count); }

	JsonValue GetChild(const char* name);
	JsonValue GetArrayIndex(uint32 index);

	JsonValue operator[](const char* name) { return GetChild(name); }
	JsonValue operator[](const String& str) { return GetChild(*str); }
	JsonValue operator[](uint32 index) { return GetArrayIndex(index); }

	bool IsValid() const { return value != nullptr; }
	bool IsArray() const;
	bool IsObject() const;
	uint32 ArrayLen() const;

private:
	rapidjson::Value* const value;
};

template<typename T>
bool JsonValue::Serialize(T& var)
{
	if (!IsValid())
		return false;

	var = value->Get<T>();
	return true;
}

template<typename T, typename TAlloc>
bool JsonValue::SerializeArray(ArrayBase<T, TAlloc>& arr)
{
	if (!IsValid())
		return false;

	if (!Assert(IsArray()))
		return false;

	uint32 len = ArrayLen();
	arr.Resize(len);
	SerializeArray(&arr[0], len);
	return true;
}

template<typename T>
bool JsonValue::SerializeArray(T* arr, uint32 count)
{
	if (!IsValid())
		return false;

	if (!Assert(IsArray()))
		return false;

	uint32 len = ArrayLen();
	len = count < len ? count : len;
	for (uint32 i = 0; i < len; ++i)
	{
		GetArrayIndex(i).Serialize<T>(arr[i]);
	}

	return true;
}
