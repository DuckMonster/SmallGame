#pragma once

class DebugPrintManager
{
public:
	struct Message
	{
		String text;
		float expireTime;
	};

	void Print(const char* message, float duration = -1.f);
	void RemoveExpired();

	const Array<Message>& GetMessages() { return messageList; }

private:
	Array<Message> messageList;
};

extern DebugPrintManager* gPrintManager;
