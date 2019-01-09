#pragma once
#include "Engine/Widget/Widget.h"

class SpacerWidget : public Widget
{
public:
	WidgetSize GetSize() override
	{
		return size;
	}

	SpacerWidget& Size(const WidgetSize& in_size)
	{
		size = in_size;
		return *this;
	}
	SpacerWidget& Size(int width, int height)
	{
		return Size(WidgetSize(width, height));
	}
	WidgetSize size;
};