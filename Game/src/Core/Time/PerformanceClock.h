#pragma once
typedef long long ptime;

class PerformanceClock
{
public:
	// Initialized the performance clock
	static void Init();

	// Gets the high-performance timestamp right now
	static ptime Now();

	// Get the tick frequency
	static ptime Frequency();

	// Get the time in seconds since some point
	static float SecondsSince(ptime from);

	// Gets the delta seconds between two time stamps
	static float GetDeltaSeconds(ptime from, ptime to);
};