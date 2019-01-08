#pragma once
#include "Structs.h"

struct WidgetDrawData
{
	Mat4 screen;
	WidgetPoint point;
};

enum class WidgetVisibility
{
	Visible,
	Hidden
};

class Widget
{
public:
	virtual ~Widget() {}

	virtual void Update() {}
	virtual void Draw(const WidgetDrawData& data) {}

	virtual WidgetSize GetSize() { return WidgetSize(); }
	virtual bool ShouldDraw() { return visibility == WidgetVisibility::Visible; }

	Widget& Visibility(WidgetVisibility inVisibility)
	{
		visibility = inVisibility;
		return *this;
	}
	WidgetVisibility visibility = WidgetVisibility::Visible;
};