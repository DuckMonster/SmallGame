#pragma once
#include "PerformanceClock.h"

class TimeStamp
{
public:
	TimeStamp();
	float Duration();

private:
	ptime init_time;
};
