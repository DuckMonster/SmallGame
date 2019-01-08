#pragma once
#include "Core/System/System.h"
#include "Engine/Widget/Container/VerticalBoxWidget.h"

struct Material;
class PerformanceWidget;
class InspectorWidget;

class GuiSystem : public System
{
public:
	void Setup() override;

protected:
	void Run() override;

private:
	Mat4 matrix;

	VerticalBoxWidget root;
	PerformanceWidget* performance;
	InspectorWidget* inspector;
};
