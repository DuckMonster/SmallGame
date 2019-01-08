
#include "DebugPrintManager.h"
#include "Core/Time/Time.h"

DebugPrintManager* gPrintManager = nullptr;

void DebugPrintManager::Print(const char* message, float duration /*= -1.f*/)
{
	Message& msg = messageList.AddRef();
	msg.text = message;
	msg.expireTime = Time::Duration() + duration;
}

void DebugPrintManager::RemoveExpired()
{
	float time = Time::Duration();
	for (uint32 i = 0; i < messageList.Size(); ++i)
	{
		if (messageList[i].expireTime <= time)
		{
			messageList.RemoveAt(i);
			i--;
		}
	}
}
