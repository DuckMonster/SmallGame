#include "PrintWidget.h"
#include "Engine/Debug/DebugPrintManager.h"

PrintWidget::PrintWidget()
{
	text = slot.AddWidget<TextWidget>();
}

void PrintWidget::Update()
{
	SlottedWidget::Update();
	const Array<DebugPrintManager::Message>& messages = gPrintManager->GetMessages();

	text->text = "{print}";

	uint32 num_messages = Math::Min(MAX_MESSAGES, messages.Size());

	// Write out messages in reverse chronological order
	for (int i = num_messages - 1; i >= 0; --i)
	{
		text->text += messages[i].text + "\n";
	}

	if (num_messages == MAX_MESSAGES)
	{
		text->text += TString::Printf(
			"{red}[MAX NUMBER OF MESSAGES REACHED (showing (%d/%d))]{/}",
			num_messages,
			messages.Size()
		);
	}

	gPrintManager->RemoveExpired();
}
