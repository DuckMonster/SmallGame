#pragma once
class Hash
{
public:
	static uint32 Get(const void* data, uint32 length);

	// Hashing some arbitrary class
	template<typename T>
	static uint32 Get(const T& data)
	{
		return Get(&data, sizeof(T));
	}

	// String hash
	template<typename T>
	static uint32 Get(const StringBase<T>& str)
	{
		return Get(*str, str.Length());
	}
};
