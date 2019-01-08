
#include "GuiSystem.h"
#include "Core/Render/Material.h"
#include "Core/Context/Context.h"
#include "Core/Resource/ResourceManager.h"
#include "Core/Resource/MaterialResource.h"
#include "Engine/Widget/Alignment/SpacerWidget.h"
#include "Engine/Widget/Debug/PerformanceWidget.h"
#include "Engine/Widget/Debug/PrintWidget.h"
#include "Engine/Widget/Debug/InspectorWidget.h"

namespace
{
	void ToggleVisiblity(Widget* widget)
	{
		widget->visibility =
			widget->visibility == WidgetVisibility::Hidden ?
			WidgetVisibility::Visible : WidgetVisibility::Hidden;
	}
}

void GuiSystem::Setup()
{
	performance = root.AddSlot().Padding(0, 0, 0, 20).AddWidget<PerformanceWidget>();
	inspector = root.AddSlot().Padding(0, 0, 0, 20).AddWidget<InspectorWidget>();
	inspector->SetScene(GetScene());
	inspector->visibility = WidgetVisibility::Hidden;
	root.AddSlot().Padding(0, 0, 0, 20).AddWidget<PrintWidget>();
}

void GuiSystem::Run()
{
	InputState& input = gContext->input;
	if (input.GetKeyPressed(Key::F1))
		ToggleVisiblity(&root);
	if (input.GetKeyPressed(Key::F2))
		ToggleVisiblity(inspector);

	WidgetDrawData data;
	data.screen = Mat4::Ortho(0.f, (float)gContext->width / 2.f, (float)gContext->height / 2.f, 0.f, -1.f, 1.f);
	data.point = WidgetPoint(10, 10);

	if (root.ShouldDraw())
	{
		root.Update();
		root.Draw(data);
	}
}