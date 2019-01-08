
#include "PerformanceClock.h"
#include <windows.h>

namespace
{
	LARGE_INTEGER clockFrequency;
}

void PerformanceClock::Init()
{
	QueryPerformanceFrequency(&clockFrequency);
}

ptime PerformanceClock::Now()
{
	LARGE_INTEGER now;
	QueryPerformanceCounter(&now);

	return now.QuadPart;
}

ptime PerformanceClock::Frequency()
{
	return clockFrequency.QuadPart;
}

float PerformanceClock::SecondsSince(ptime from)
{
	return GetDeltaSeconds(from, Now());
}

float PerformanceClock::GetDeltaSeconds(ptime from, ptime to)
{
	LARGE_INTEGER elapsedMicroseconds;
	elapsedMicroseconds.QuadPart = to - from;

	float Delta = (float)elapsedMicroseconds.QuadPart / clockFrequency.QuadPart;
	return Delta;
}
