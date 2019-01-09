
#include "PerformanceClock.h"
#include <windows.h>

namespace
{
	LARGE_INTEGER clock_frequency;
}

void PerformanceClock::Init()
{
	QueryPerformanceFrequency(&clock_frequency);
}

ptime PerformanceClock::Now()
{
	LARGE_INTEGER now;
	QueryPerformanceCounter(&now);

	return now.QuadPart;
}

ptime PerformanceClock::Frequency()
{
	return clock_frequency.QuadPart;
}

float PerformanceClock::SecondsSince(ptime from)
{
	return GetDeltaSeconds(from, Now());
}

float PerformanceClock::GetDeltaSeconds(ptime from, ptime to)
{
	LARGE_INTEGER elapsed_us;
	elapsed_us.QuadPart = to - from;

	float Delta = (float)elapsed_us.QuadPart / clock_frequency.QuadPart;
	return Delta;
}
