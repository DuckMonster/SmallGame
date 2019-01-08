#pragma once
#include "Engine/GUI/Text/TextMesh.h"
#include "Engine/Widget/Widget.h"

class TextWidget : public Widget
{
public:
	TextWidget();
	void Update() override;
	void Draw(const WidgetDrawData& data) override;
	WidgetSize GetSize() override;

	void SetStyle(const char* path);

	String text;

private:
	String stylePath;
	String prevText;
	TextMesh mesh;
};
