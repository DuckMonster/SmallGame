#pragma once

class Color
{
public:
	/* Standard colors */
	static const Color white;
	static const Color black;
	static const Color transparent;
	static const Color red;
	static const Color green;
	static const Color blue;
	static const Color orange;
	static const Color purple;
	static const Color cyan;
	static const Color yellow;

	static Color FromHex(uint32 hex);
	static Color FromBytes(uint8 r, uint8 g, uint8 b, uint8 a);

	/* Constructors */
	Color() : r(1.f), g(1.f), b(1.f), a(1.f) {}
	Color(float r, float g, float b, float a) :
		r(r), g(g), b(b), a(a) {}

	Color operator*(const Color& other)
	{
		return Color(r * other.r, g * other.g, b * other.b, a * other.a);
	}

	// Clamps the color so that all values are between [0, 1]
	void Clamp();

	float r;
	float g;
	float b;
	float a;
};
