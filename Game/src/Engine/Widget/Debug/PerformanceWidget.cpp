
#include "PerformanceWidget.h"
#include "Engine/Widget/Container/VerticalBoxWidget.h"

PerformanceWidget::PerformanceWidget()
{
	VerticalBoxWidget* box = slot.AddWidget<VerticalBoxWidget>();

	fps =
		box->AddSlot().AddWidget<TextWidget>();
	storage =
		box->AddSlot().AddWidget<TextWidget>();
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
		float realMs = Time::Delta() * 1000.f;

		// Has time window finished?
		if (Time::Duration() > resetTime)
			Reset();

		// Update max and min
		if (ms > maxMs)
			maxMs = ms;
		if (ms < minMs || minMs == -1.f)
			minMs = ms;

		TString str;
		str += TString::Printf("MS: %s (%s) (%s - %s)",
			*TagHelper(ms),
			*TagHelper(realMs),
			*TagHelper(minMs),
			*TagHelper(maxMs)
		);

		fps->text = str;
	}

	/*-- UPDATE STORAGE --*/
	{
		storage->text = TString::Printf("{cyan}TEMP STACK %d/%d B{}", gTempStack->prevOffset, gTempStack->dataSize);
	}
}