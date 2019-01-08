#pragma once
#include "Engine/Widget/SlottedWidget.h"
#include "Engine/Widget/Text/TextWidget.h"

class PrintWidget : public SlottedWidget
{
public:
	PrintWidget();
	void Update() override;

private:
	static const uint32 MAX_MESSAGES = 25;
	TextWidget* text;
};
