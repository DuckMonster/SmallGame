
#include "Time.h"
#include "PerformanceClock.h"

static ptime initTime;
static ptime lastFrameTime;
static float lastFrameDelta;

void Time::Init()
{
	initTime = PerformanceClock::Now();
	lastFrameTime = initTime;
}

void Time::Update()
{
	ptime now = PerformanceClock::Now();
	lastFrameDelta = PerformanceClock::GetDeltaSeconds(lastFrameTime, now);
	lastFrameTime = now;
}

float Time::Delta()
{
	return lastFrameDelta;
}

float Time::Duration()
{
	return PerformanceClock::GetDeltaSeconds(initTime, PerformanceClock::Now());
}

static ptime frameBeginTime;
static float lastFrameRealTime;
void Time::BeginFrame()
{
	frameBeginTime = PerformanceClock::Now();
}

void Time::EndFrame()
{
	lastFrameRealTime = PerformanceClock::SecondsSince(frameBeginTime);
}

float Time::InternalFrameTime()
{
	return lastFrameRealTime;
}
