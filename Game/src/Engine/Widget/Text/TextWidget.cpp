
#include "TextWidget.h"
#include "Core/Resource/ResourceManager.h"
#include "Engine/GUI/Style/StyleResource.h"

TextWidget::TextWidget() : Widget()
{
	SetStyle("Style/default.json");
	mesh.Create();
}

void TextWidget::Update()
{
	// Update not needed
	if (text == prevText)
		return;

	mesh.BuildString(*text);
	prevText = text;
}

void TextWidget::Draw(const WidgetDrawData& data)
{
	mesh.Draw(data.point.x, data.point.y, data.screen);
}

WidgetSize TextWidget::GetSize()
{
	return WidgetSize(mesh.width, mesh.height);
}

void TextWidget::SetStyle(const char* path)
{
	stylePath = path;
	mesh.style = gResourceManager->Load<StyleResource>(path);
}
