
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
	WidgetDrawData data_cur = data;

	for (Slot& slot : slots)
	{
		if (slot.widget == nullptr)
			continue;

		if (slot.widget->visibility == WidgetVisibility::Hidden)
			continue;

		// Add upper-left padding
		data_cur.point.x += slot.padding.left;
		data_cur.point.y += slot.padding.up;

		slot.widget->Draw(data_cur);

		data_cur.point.y += slot.widget->GetSize().height;

		// Add lower and remove left padding
		data_cur.point.x -= slot.padding.left;
		data_cur.point.y += slot.padding.down;
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

		WidgetSize widget_size = slot.widget->GetSize();

		if (widget_size.width > size.width)
			size.width = widget_size.width;
		size.height += widget_size.height;
	}

	return size;
}
