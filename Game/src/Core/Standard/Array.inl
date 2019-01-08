template<typename InType, typename InAllocator>
char ArrayBase<InType, InAllocator>::ERROR_VALUE[sizeof(InType)];

template<typename InType, typename InAllocator>
InType& ArrayBase<InType, InAllocator>::GetErrorValue()
{
	return *(InType*)ERROR_VALUE;
}

template<typename InType, typename InAllocator>
InType& ArrayBase<InType, InAllocator>::AddUnique(const InType& value)
{
	uint32 index;
	if (Find(value, index))
		return data[index];

	return Add(value);
}

template<typename InType, typename InAllocator>
InType& ArrayBase<InType, InAllocator>::Add(const InType& value)
{
	return Emplace(value);
}


template<typename InType, typename InAllocator>
InType& ArrayBase<InType, InAllocator>::AddRef()
{
	return Emplace();
}

template<typename InType, typename InAllocator>
template<typename... InArgs>
InType& ArrayBase<InType, InAllocator>::Emplace(const InArgs&... args)
{
	Reserve(size + 1);

	// Construct object before assignment
	new(data + size) InType(args...);
	InType& result = data[size];

	size++;
	return result;
}

template<typename InType, typename InAllocator>
bool ArrayBase<InType, InAllocator>::Remove(const InType& value)
{
	uint32 index;
	if (!Find(value, index))
		return false;

	return RemoveAt(index);
}

template<typename InType, typename InAllocator>
bool ArrayBase<InType, InAllocator>::RemoveAt(uint32 index)
{
	if (index >= size)
	{
		Error("Tried removing index %d in an array with size %d", index, size);
		return false;
	}

	// Run destructor on object being removed
	data[index].~InType();

	uint32 lastIndex = size - 1;
	if (index < lastIndex)
	{
		// If the removed index is not at the end, move everything after index down one step
		memmove(data + index, data + index + 1, sizeof(InType) * (lastIndex - index));
	}

	size--;
	return true;
}

template<typename InType, typename InAllocator>
bool ArrayBase<InType, InAllocator>::Find(const InType& value, uint32& outIndex)
{
	for (uint32 i = 0; i < size; ++i)
	{
		if (data[i] == value)
		{
			outIndex = i;
			return true;
		}
	}

	return false;
}

template<typename InType, typename InAllocator>
bool ArrayBase<InType, InAllocator>::Contains(const InType& value)
{
	uint32 dummy;
	return Find(value, dummy);
}

template<typename InType, typename InAllocator>
InType& ArrayBase<InType, InAllocator>::operator[](uint32 index)
{
	if (index >= size)
	{
		Fatal("Tried accessing index [%d] in an array of size [%d]", index, size);
		return GetErrorValue();
	}

	return data[index];
}

template<typename InType, typename InAllocator>
const InType& ArrayBase<InType, InAllocator>::operator[](uint32 index) const
{
	if (index >= size)
	{
		Fatal("Tried accessing index [%d] in an array of size [%d]", index, size);
		return GetErrorValue();
	}

	return data[index];
}
