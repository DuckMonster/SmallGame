#pragma once

class Random
{
public:
	static uint32 GetSeed();
	static void SetSeed(uint32 seed);

	// Returns a random uint from 0 to RAND_MAX
	static uint32 Int();

	// Returns a random float between 0.f and 1.f
	static float Value();

	// Returns a random float between min and max
	static float Range(float min, float max);

	// Returns a random in between min and max
	static int Range(int min, int max);

	// Returns a random unit vec2
	static Vec2 UnitVec2();

	// Returns a random unit vec3
	static Vec3 UnitVec3();
};