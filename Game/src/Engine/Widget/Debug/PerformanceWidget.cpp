
#include "PerformanceWidget.h"
#include "Engine/Widget/Container/VerticalBoxWidget.h"

PerformanceWidget::PerformanceWidget()
{
	VerticalBoxWidget* box = slot.AddWidget<VerticalBoxWidget>();

	fps 	= box->AddSlot().AddWidget<TextWidget>();
	storage = box->AddSlot().AddWidget<TextWidget>();
}

void PerformanceWidget::Update()
{
	SlottedWidget::Update();

	/* -- UPDATE FPS --*/
	{
		/* Helper to create a formatted string for ms (colored based on how big it is) */
		struct
		{
			TString operator()(float ms)
			{
				TString tag;
				if (ms < 4.f)
					tag = "green";
				else if (ms < 10.f)
					tag = "yellow";
				else
					tag = "red";

				return TString::Printf("{%s}%.2f ms{}", *tag, ms);
			}
		} TagHelper;

		// Update ms
		float ms = Time::InternalFrameTime() * 1000.f;
		float real_ms = Time::Delta() * 1000.f;

		// Has time window finished?
		if (Time::Duration() > reset_time)
			Reset();

		// Update max and min
		if (ms > max_ms)
			max_ms = ms;
		if (ms < min_ms || min_ms == -1.f)
			min_ms = ms;

		TString str;
		str += TString::Printf("MS: %s (%s) (%s - %s)",
			*TagHelper(ms),
			*TagHelper(real_ms),
			*TagHelper(min_ms),
			*TagHelper(max_ms)
		);

		fps->text = str;
	}

	/*-- UPDATE STORAGE --*/
	{
		storage->text = TString::Printf("{cyan}TEMP STACK %d/%d B{}", temp_stack.prev_offset, temp_stack.data_size);
	}
}