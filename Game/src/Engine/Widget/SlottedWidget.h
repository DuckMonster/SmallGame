#pragma once
#include "Widget.h"
#include "Slot/WidgetSlot.h"

/* A widget with a single slot in it,
	meant to be used for creating new custom widgets by
	combining existing widgets together */
class SlottedWidget : public Widget
{
public:
	~SlottedWidget();
	virtual void Update() override;
	virtual void Draw(const WidgetDrawData& data) override;
	virtual WidgetSize GetSize() override;

	WidgetSlot slot;
};
