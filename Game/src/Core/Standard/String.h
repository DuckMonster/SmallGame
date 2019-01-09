#pragma once
#include "Core/Memory/Allocator.h"
#include <string>

template<typename InAllocator>
class StringBase
{
	template<typename InOtherAllocator>
	friend class StringBase;

public:
	static StringBase Printf(const char* format, ...);
	static StringBase VPrintf(const char* format, va_list list);

public:
	StringBase() { Set(""); }
	StringBase(const char* str) { Set(str); }
	StringBase(const std::string& str) { Set(str.c_str()); }
	StringBase(const StringBase& other) { Set(*other); }
	StringBase(StringBase&& other) { Move(std::move(other));  }

	// Allocator conversions
	template<typename InOtherAllocator>
	StringBase(const StringBase<InOtherAllocator>& other) { Set(*other); }

	~StringBase()
	{
		if (data != nullptr)
		{
			InAllocator::Free(data);
			size = 0;
			length = 0;
		}
	}

	// Appends the specified string to the end of this one
	void Add(const char* str)
	{
		uint32 len = strlen(str);
		Reserve(length + len);

		memcpy(data + length, str, len + 1);
		length += len;
	}

	// Copy string into this object
	void Set(const char* str)
	{
		uint32 len = strlen(str);
		Reserve(len);

		// +1 to include null terminator
		memcpy(data, str, len + 1);
		length = len;
	}

	// Copy string into this object, but only copy length bytes
	void Set(const char* str, uint32 in_length)
	{
		if (in_length > strlen(str))
		{
			Error("Setting string with length longer than the string");
			in_length = strlen(str);
		}

		Reserve(in_length);
		memcpy(data, str, in_length);

		// Manually include null terminator
		data[in_length] = '\0';
		length = in_length;
	}

	// Returns the length (amount of characters) of the string
	uint32 Length() const { return length; }

	// Will reserve memory for (at least) a string of new_length length
	void Reserve(uint32 new_length)
	{
		if (new_length + 1 > size)
		{
			char* old_data = data;

			data = (char*)InAllocator::Malloc(new_length + 1);

			if (old_data)
			{
				memcpy(data, old_data, size);
				InAllocator::Free(old_data);
			}

			size = new_length + 1;
		}
	}

	// Returns if the string is empty
	bool IsEmpty() { return length == 0; }

	// Copies a section of this string into a new one
	StringBase SubStr(uint32 offset, uint32 count)
	{
		if (offset >= length)
		{
			Error("Offset is outside string length");
			return StringBase();
		}
		if (offset + count > length)
		{
			Error("Can't substr section [%d - %d] in a string of length [%d]", offset, offset + count, length);
			return StringBase();
		}

		// Memcpy the section into a new string
		StringBase result;
		result.Set(data + offset, count);
		return result;
	}

	StringBase& operator=(const char* str) { Set(str); return *this; }
	StringBase& operator=(const StringBase& other) { return operator=(*other); }
	StringBase& operator=(StringBase&& other) { Move(std::move(other)); return *this; }
	// Allocator conversions
	template<typename InOtherAllocator>
	StringBase& operator=(const StringBase<InOtherAllocator>& other) { return operator=(*other); }

	StringBase operator+(const char* str) const
	{
		StringBase result(*this);
		result.Add(str);
		return result;
	}
	template<typename InOtherAllocator>
	StringBase operator+(const StringBase<InOtherAllocator>& other) const { return operator+(*other); }
	StringBase& operator+=(const char* str)
	{
		Add(str);
		return *this;
	}
	template<typename InOtherAllocator>
	StringBase& operator+=(const StringBase<InOtherAllocator>& other) { return operator+=(*other); }

	bool operator==(const char* str) const
	{
		return strcmp(data, str) == 0;
	}
	template<typename InOtherAllocator>
	bool operator==(const StringBase<InOtherAllocator>& other) const { return operator==(*other); }

	bool operator!=(const char* str) const { return !operator==(str); }
	template<typename InOtherAllocator>
	bool operator!=(const StringBase<InOtherAllocator>& other) const { return !operator==(*other); }

	char& operator[](uint32 index) { Assert(index < length); return data[index]; }
	const char& operator[](uint32 index) const { Assert(index < length); return data[index]; }

	const char* operator*() const { return data; }

private:
	// Will move data from an rvalue into this string (and invalidating the rvalue)
	void Move(StringBase&& other)
	{
		// Free our current memory, since it will be overwritten
		if (data != nullptr)
			InAllocator::Free(data);

		data = other.data;
		size = other.size;
		length = other.length;

		other.data = nullptr;
		other.size = 0;
		other.length = 0;
	}

	char* data = nullptr;
	uint32 size = 0;
	uint32 length = 0;
};

typedef StringBase<HeapAllocator> String;
typedef StringBase<TemporaryAllocator> TString;
