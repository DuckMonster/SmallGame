
#include "VerticalBoxWidget.h"

void VerticalBoxWidget::Update()
{
	Widget::Update();
	for (Slot& slot : slots)
	{
		if (slot.widget != nullptr)
			slot.widget->Update();
	}
}

void VerticalBoxWidget::Draw(const WidgetDrawData& data)
{
	Widget::Draw(data);
	WidgetDrawData curData = data;

	for (Slot& slot : slots)
	{
		if (slot.widget == nullptr)
			continue;

		if (slot.widget->visibility == WidgetVisibility::Hidden)
			continue;

		// Add upper-left padding
		curData.point.x += slot.padding.left;
		curData.point.y += slot.padding.up;

		slot.widget->Draw(curData);

		curData.point.y += slot.widget->GetSize().height;

		// Add lower and remove left padding
		curData.point.x -= slot.padding.left;
		curData.point.y += slot.padding.down;
	}
}

void VerticalBoxWidget::ClearChildren()
{
	for (Slot& slot : slots)
	{
		slot.Clear();
	}

	slots.Clear();
}

WidgetSize VerticalBoxWidget::GetSize()
{
	WidgetSize size;
	for (Slot& slot : slots)
	{
		if (slot.widget == nullptr)
			continue;

		if (slot.widget->visibility == WidgetVisibility::Hidden)
			continue;

		WidgetSize wSize = slot.widget->GetSize();

		if (wSize.width > size.width)
			size.width = wSize.width;
		size.height += wSize.height;
	}

	return size;
}
