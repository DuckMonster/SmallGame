
#include "DebugPrintManager.h"
#include "Core/Time/Time.h"

DebugPrintManager* gPrintManager = nullptr;

void DebugPrintManager::Print(const char* message, float duration /*= -1.f*/)
{
	Message& msg = messages.AddRef();
	msg.text = message;
	msg.expire_time = Time::Duration() + duration;
}

void DebugPrintManager::RemoveExpired()
{
	float time = Time::Duration();
	for (uint32 i = 0; i < messages.Size(); ++i)
	{
		if (messages[i].expire_time <= time)
		{
			messages.RemoveAt(i);
			i--;
		}
	}
}
