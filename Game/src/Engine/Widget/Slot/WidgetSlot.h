#pragma once
#include "Engine/Widget/Widget.h"

class WidgetSlot
{
public:
	template<typename T>
	T* AddWidget()
	{
		if (widget != nullptr)
		{
			Error("A widget is already occupying this slot");
			delete widget;
		}

		T* twidget = new T();
		widget = twidget;
		return twidget;
	}

	void Clear()
	{
		if (widget != nullptr)
		{
			delete widget;
			widget = nullptr;
		}
	}

	Widget* widget = nullptr;
};
