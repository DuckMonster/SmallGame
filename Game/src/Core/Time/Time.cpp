
#include "Time.h"
#include "PerformanceClock.h"

static ptime init_time;
static ptime last_frame_time;
static float last_frame_delta;

void Time::Init()
{
	init_time = PerformanceClock::Now();
	last_frame_time = init_time;
}

void Time::Update()
{
	ptime now = PerformanceClock::Now();
	last_frame_delta = PerformanceClock::GetDeltaSeconds(last_frame_time, now);
	last_frame_time = now;

	// Limit delta to 100 ms
	if (last_frame_delta > 0.1f)
	{
		Debug_Log("Frame delta clamped (%f)", last_frame_delta);
		last_frame_delta = 0.1f;
	}
}

float Time::Delta()
{
	return last_frame_delta;
}

float Time::Duration()
{
	return PerformanceClock::GetDeltaSeconds(init_time, PerformanceClock::Now());
}

static ptime frame_begin_time;
static float last_frame_real_time;
void Time::BeginFrame()
{
	frame_begin_time = PerformanceClock::Now();
}

void Time::EndFrame()
{
	last_frame_real_time = PerformanceClock::SecondsSince(frame_begin_time);
}

float Time::InternalFrameTime()
{
	return last_frame_real_time;
}
