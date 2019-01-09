#pragma once
#include "Engine/Widget/Widget.h"
#include "Engine/Widget/Slot/WidgetSlot.h"

class VerticalBoxWidget : public Widget
{
public:
	class Slot : public WidgetSlot
	{
	public:
		Slot& Padding(const WidgetPadding& in_padding)
		{
			padding = in_padding;
			return *this;
		}
		Slot& Padding(int uniform)
		{
			return Padding(WidgetPadding(uniform));
		}
		Slot& Padding(int horizontal, int vertical)
		{
			return Padding(WidgetPadding(horizontal, vertical));
		}
		Slot& Padding(int left, int up, int right, int down)
		{
			return Padding(WidgetPadding(left, up, right, down));
		}

		WidgetPadding padding;
	};

	void Update() override;
	void Draw(const WidgetDrawData& data) override;
	void ClearChildren();

	WidgetSize GetSize() override;

	Slot& AddSlot()
	{
		return slots.AddRef();
	}

private:
	Array<Slot> slots;
};
