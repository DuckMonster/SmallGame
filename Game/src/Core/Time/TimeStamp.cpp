
#include "TimeStamp.h"

TimeStamp::TimeStamp()
{
	initTime = PerformanceClock::Now();
}

float TimeStamp::Duration()
{
	return PerformanceClock::SecondsSince(initTime);
}
