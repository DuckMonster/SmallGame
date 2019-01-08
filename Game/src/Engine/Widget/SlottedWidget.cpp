
#include "SlottedWidget.h"

SlottedWidget::~SlottedWidget()
{
	slot.Clear();
}

void SlottedWidget::Update()
{
	if (slot.widget != nullptr)
		slot.widget->Update();
}

void SlottedWidget::Draw(const WidgetDrawData& data)
{
	if (slot.widget != nullptr && slot.widget->ShouldDraw())
		slot.widget->Draw(data);
}

WidgetSize SlottedWidget::GetSize()
{
	if (slot.widget != nullptr && slot.widget->ShouldDraw())
		return slot.widget->GetSize();

	return Widget::GetSize();
}
