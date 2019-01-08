
#include "String.h"
#include <stdarg.h>

template<typename InAllocator>
StringBase<InAllocator> StringBase<InAllocator>::Printf(const char* format, ...)
{
	StringBase result;

	va_list list;
	va_start(list, format);

	int len = vsnprintf(nullptr, 0, format, list);
	result.Reserve(len);
	vsprintf_s(result.data, len + 1, format, list);
	va_end(list);

	return result;
}

template<typename InAllocator>
StringBase<InAllocator> StringBase<InAllocator>::VPrintf(const char* format, va_list list)
{
	StringBase result;

	int len = vsnprintf(nullptr, 0, format, list);
	result.Reserve(len);
	vsprintf_s(result.data, len + 1, format, list);

	return result;
}

template class StringBase<HeapAllocator>;
template class StringBase<TemporaryAllocator>;
