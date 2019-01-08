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
		minMs = -1.f;
		maxMs = 0.f;
		resetTime += 2.f;
	}

	float minMs = -1.f;
	float maxMs = 0.f;
	float resetTime = 0.f;

	TextWidget* fps;

	// Storage monitoring stuff
	TextWidget* storage;
};
