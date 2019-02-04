#include "Random.h"
#include <time.h>
#include <stdlib.h>

uint32 Random::GetSeed()
{
	// Not implemented
	return 0;
}

void Random::SetSeed(uint32 seed)
{
	srand(seed);
}

// Returns a random int from 0 to RAND_MAX
uint32 Random::Int()
{
	return rand();
}

// Returns a random float between 0.f and 1.f
float Random::Value()
{
	return rand() / (float)RAND_MAX;
}

// Returns a random float between min and max
float Random::Range(float min, float max)
{
	return min + Random::Value() * (max - min);
}

// Returns a random in between min and max
int Random::Range(int min, int max)
{
	return min + rand() % (max - min);
}

// Returns a random unit vec2
Vec2 Random::UnitVec2()
{
	Vec2 v;
	float len = 0.f;

	do
	{
		v.x = Random::Range(-1.f, 1.f);
		v.y = Random::Range(-1.f, 1.f);

		len = Vec::LengthSqrd(v);
	} while(len > 1.f || len < KINDA_SMALL_NUMBER);

	return v / Math::Sqrt(len);
}

// Returns a random unit vec3
Vec3 Random::UnitVec3()
{
	Vec3 v;
	float len = 0.f;

	do
	{
		v.x = Random::Range(-1.f, 1.f);
		v.y = Random::Range(-1.f, 1.f);
		v.z = Random::Range(-1.f, 1.f);

		len = Vec::LengthSqrd(v);
	} while(len > 1.f || len < KINDA_SMALL_NUMBER);

	return v / Math::Sqrt(len);
}