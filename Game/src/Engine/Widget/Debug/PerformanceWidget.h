#pragma once
#include "Engine/GUI/Text/TextMesh.h"
#include "Engine/Widget/SlottedWidget.h"
#include "Engine/Widget/Text/TextWidget.h"

class PerformanceWidget : public SlottedWidget
{
public:
	PerformanceWidget();
	void Update() override;

private:
	// FPS monitoring stuff
	void Reset()
	{
		min_ms = -1.f;
		max_ms = 0.f;
		reset_time += 2.f;
	}

	float min_ms = -1.f;
	float max_ms = 0.f;
	float reset_time = 0.f;

	TextWidget* fps;

	// Storage monitoring stuff
	TextWidget* storage;
};
