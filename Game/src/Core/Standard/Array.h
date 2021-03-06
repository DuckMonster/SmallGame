#pragma once
#include <cstring>
#include "Core/Memory/Allocator.h"

template<typename InType, typename InAllocator>
class ArrayBase
{
	template<typename InOtherType, typename InOtherAllocator>
	friend class ArrayBase;

public:
	class Iterator
	{
	public:
		Iterator(InType* data, uint32 index = 0) :
			data(data), index(index) { }
		Iterator(const Iterator& other) : data(other.data), index(other.index) {}
		~Iterator() {}

		Iterator& operator++()
		{
			index++;
			return *this;
		}
		InType& operator*() const
		{
			return data[index];
		}

		bool operator!=(const Iterator& other)
		{
			return !operator==(other);
		}
		bool operator==(const Iterator& other)
		{
			return data == other.data && index == other.index;
		}

		InType* data;
		uint32 index;
	};

	class ConstIterator
	{
	public:
		ConstIterator(const InType* data, uint32 index = 0) :
			data(data), index(index) { }
		ConstIterator(const Iterator& other) : data(other.data), index(other.index) {}
		~ConstIterator() {}

		ConstIterator& operator++()
		{
			index++;
			return *this;
		}
		const InType& operator*() const
		{
			return data[index];
		}

		bool operator!=(const ConstIterator& other)
		{
			return !operator==(other);
		}
		bool operator==(const ConstIterator& other)
		{
			return data == other.data && index == other.index;
		}

		const InType* data;
		uint32 index;
	};

public:
	ArrayBase() {}
	ArrayBase(const ArrayBase& other)
	{
		Reserve(other.capacity);
		size = other.size;

		// Call copy constructor for every element
		for (uint32 i = 0; i < other.size; ++i)
		{
			new(data + i) InType(other.data[i]);
		}
	}
	ArrayBase(ArrayBase&& other)
	{
		data = other.data;
		capacity = other.capacity;
		size = other.size;

		other.data = nullptr;
		other.capacity = 0;
		other.size = 0;
	}
	// Allocator conversion
	template<typename InOtherAlloc>
	ArrayBase(const ArrayBase<InType, InOtherAlloc>& other)
	{
		Reserve(other.capacity);
		size = other.size;

		// Call copy constructor for every element
		for (uint32 i = 0; i < other.size; ++i)
		{
			new(data + i) InType(other.data[i]);
		}
	}
	~ArrayBase()
	{
		Clear();

		if (data != nullptr)
			InAllocator::Free(data);
	}

	// Adds an element at the end of the array
	InType& Add(const InType& value);
	// Only adds an element if it doesnt already exist
	InType& AddUnique(const InType& value);

	// Adds (with default value) and returns a ref to the new object
	InType& AddRef();

	// Inserts an element at some index
	InType& Insert(uint32 index, const InType& value)
	{
		return InsertEmplace(index, value);
	}
	InType& InsertRef(uint32 index)
	{
		return InsertEmplace(index);
	}
	template<typename... InArgs>
	InType& InsertEmplace(uint32 index, InArgs... args)
	{
		if (index > size)
		{
			Error("Insert index outsize array bounds");
			return GetErrorValue();
		}
		
		Reserve(size + 1);

		// Move all elements above the inserted index
		memmove(data + index + 1, data + index, sizeof(InType) * (size - index));
		new(data + index) InType(args...);

		size++;
		return data[index];
	}
	InType& InsertUnitialized(uint32 index)
	{
		if (index > size)
		{
			Error("Insert index outsize array bounds");
			return GetErrorValue();
		}
		
		Reserve(size + 1);

		// Move all elements above the inserted index
		memmove(data + index + 1, data + index, sizeof(InType) * (size - index));

		size++;
		return data[index];
	}

	// Adds an element at the end of the array, but doesn't initialize it (call its constructor)
	InType& AddUninitialized()
	{
		Reserve(size + 1);
		InType& item = data[size];
		size++;

		return item;
	}

	// Adds and element and initializes it with the specified args (calls its constructor)
	template<typename... InArgs>
	InType& Emplace(const InArgs&...);

	// Removes the first occurance of value in the array
	bool Remove(const InType& value);
	// Removes the element at a specific index (realigns everything afterwards)
	bool RemoveAt(uint32 index);
	// Finds the index of a specific value (and returns if it found anything)
	bool Find(const InType& value, uint32& out_index);
	// Returns if this array contains a specific value
	bool Contains(const InType& value);

	// Returns if the array has no elements in it
	bool IsEmpty() const { return size == 0; }
	// Empties the array
	void Clear() { Resize(0); }

	// Empties the array, but doesnt destroy any objects (call destructor)
	void ClearNoDestruct()
	{
		size = 0;
	}

	// Returns how many objects are in this array
	uint32 Size() const
	{
		return size;
	}

	// Resizes the array and adds/removed objects to fit
	void Resize(uint32 in_size, const InType& value = InType())
	{
		ResizeEmplace(in_size, value);
	}
	// Resizes the array and adds/removed objects to fit (calls the constructor with args when adding)
	template<typename... TArgs>
	void ResizeEmplace(uint32 in_size, const TArgs&... args)
	{
		// Same size, we dont care
		if (in_size == size)
			return;

		// Make sure our buffer is big enough
		Reserve(in_size);

		// If we want the array to be bigger, make sure to construct objects
		if (in_size > size)
		{
			for (uint32 i = size; i < in_size; ++i)
				new(data + i) InType(args...);
		}
		// Otherwise, destruct objects that will get deleted
		else
		{
			for (uint32 i = in_size; i < size; ++i)
			{
				data[i].~InType();
			}
		}

		size = in_size;
	}

	// Will resize the internal buffer to fit in_capacity number of elements, but wont actually grow the array
	void Reserve(uint32 in_capacity)
	{
		// Only grow if bigger
		if (capacity < in_capacity)
		{
			InType* old_data = data;
			data = (InType*)InAllocator::Malloc(sizeof(InType) * in_capacity);

			if (old_data != nullptr)
			{
				// Copy data from old buffer to new
				memcpy(data, old_data, sizeof(InType) * capacity);
				InAllocator::Free(old_data);
			}

			capacity = in_capacity;
		}
	}

	// = operators
	ArrayBase& operator=(const ArrayBase& other)
	{
		Clear();
		Reserve(other.size);

		for(uint32 i=0; i<other.size; ++i)
			new(data + i) InType(other[i]);

		size = other.size;
		return *this;
	}
	ArrayBase& operator=(ArrayBase&& other)
	{
		// For rvalues, we just wanna transfer the memory from the other array
		// Clear and de-allocate memory
		Clear();
		if (data != nullptr)
			InAllocator::Free(data);

		// Then just transfer the pointers, sizes and such
		data = other.data;
		capacity = other.capacity;
		size = other.size;

		other.data = nullptr;
		other.capacity = 0;
		other.size = 0;

		return *this;
	}
	template<typename InOtherAlloc>
	ArrayBase& operator=(const ArrayBase<InType, InOtherAlloc>& other)
	{
		// Clear this array first
		Clear();
		Reserve(other.size);

		for(uint32 i=0; i<other.size; ++i)
			new(data + i) InType(other[i]);

		size = other.size;
		return *this;
	}

	InType& operator[](uint32 index);
	const InType& operator[](uint32 index) const;

	// Pointer getters
	InType* Ptr() { return data; }
	const InType* Ptr() const { return data; }
	InType* operator*() { return Ptr(); }
	const InType* operator*() const { return Ptr(); }

	// std-iterator compliant functions
	Iterator begin()
	{
		return Iterator(data, 0);
	}
	Iterator end()
	{
		return Iterator(data, size);
	}
	ConstIterator begin() const
	{
		return ConstIterator(data, 0);
	}
	ConstIterator end() const
	{
		return ConstIterator(data, size);
	}

private:
	// Error value is used when a fatal error has occured, since the error could be expected
	static char ERROR_VALUE[sizeof(InType)];
	static InType& GetErrorValue();

	InType* data = nullptr;
	uint32 capacity = 0;
	uint32 size = 0;
};

template<typename T>
using Array = ArrayBase<T, HeapAllocator>;
template<typename T>
using TArray = ArrayBase<T, TemporaryAllocator>;

#include "Array.inl"