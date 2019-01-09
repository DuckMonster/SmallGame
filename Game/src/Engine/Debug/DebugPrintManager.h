#pragma once

class DebugPrintManager
{
public:
	struct Message
	{
		String text;
		float expire_time;
	};

	void Print(const char* message, float duration = -1.f);
	void RemoveExpired();

	const Array<Message>& GetMessages() { return messages; }

private:
	Array<Message> messages;
};

extern DebugPrintManager* gPrintManager;
