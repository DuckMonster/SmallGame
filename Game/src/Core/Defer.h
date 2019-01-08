#pragma once

template<typename T>
struct ExitScope
{
	T lambda;
	ExitScope(T lambda) : lambda(lambda) {}
	~ExitScope() { lambda(); }
};

struct ExitScopeHelper
{
	template<typename T>
	ExitScope<T> operator+(T t) { return ExitScope<T>(t); }
};

#define CONCAT_INTERNAL(a,b) a##b
#define CONCAT(a,b) CONCAT_INTERNAL(a,b)
#define defer const auto& CONCAT(defer__, __LINE__) = ExitScopeHelper() + [&]()