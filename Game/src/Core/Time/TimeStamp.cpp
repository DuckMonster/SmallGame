
#include "TimeStamp.h"

TimeStamp::TimeStamp()
{
	init_time = PerformanceClock::Now();
}

float TimeStamp::Duration()
{
	return PerformanceClock::SecondsSince(init_time);
}
