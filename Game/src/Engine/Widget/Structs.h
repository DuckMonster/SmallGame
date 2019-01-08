#pragma once
struct WidgetPoint
{
	WidgetPoint() {}
	WidgetPoint(int x, int y) : x(x), y(y) {}

	int x = 0;
	int y = 0;
};

struct WidgetSize
{
	WidgetSize() {}
	WidgetSize(int width, int height) : width(width), height(height) {}

	int width = 0;
	int height = 0;
};

struct WidgetBounds
{
public:
	WidgetPoint point;
	WidgetSize size;
};

struct WidgetPadding
{
	WidgetPadding() {}
	WidgetPadding(int uniform) :
		left(uniform), up(uniform), right(uniform), down(uniform) {}

	WidgetPadding(int horizontal, int vertical) :
		left(horizontal), up(vertical), right(horizontal), down(vertical) {}

	WidgetPadding(int left, int up, int right, int down) :
		left(left), up(up), right(right), down(down) {}

	int left = 0;
	int up = 0;
	int right = 0;
	int down = 0;
};
