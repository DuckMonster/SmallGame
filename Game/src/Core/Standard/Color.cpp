
#include "Color.h"

const Color Color::white = Color::FromHex(0xFFFFFFFF);
const Color Color::black = Color::FromHex(0x000000FF);
const Color Color::transparent = Color::FromHex(0x00000000);
const Color Color::red = Color::FromHex(0xFF0000FF);
const Color Color::green = Color::FromHex(0x00FF00FF);
const Color Color::blue = Color::FromHex(0x0000FFFF);
const Color Color::orange = Color::FromHex(0xFFA400FF);
const Color Color::purple = Color::FromHex(0xFF00FFFF);
const Color Color::cyan = Color::FromHex(0x00FFFFFF);
const Color Color::yellow = Color::FromHex(0xFFFF00FF);

Color Color::FromHex(uint32 hex)
{
	struct HexHelper
	{
		uint8 a;
		uint8 b;
		uint8 g;
		uint8 r;
	};

	HexHelper* helper = (HexHelper*)&hex;
	return FromBytes(helper->r, helper->g, helper->b, helper->a);
}

Color Color::FromBytes(uint8 r, uint8 g, uint8 b, uint8 a)
{
	return Color(
		(float)r / 255.f,
		(float)g / 255.f,
		(float)b / 255.f,
		(float)a / 255.f
	);
}

void Color::Clamp()
{
	r = Math::Saturate(r);
	g = Math::Saturate(g);
	b = Math::Saturate(b);
	a = Math::Saturate(a);
}
