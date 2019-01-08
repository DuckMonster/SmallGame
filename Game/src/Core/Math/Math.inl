template<typename T>
float Math::Wrap(T val, T min, T max)
{
	if (min == max)
		return min;

	float dif = max - min;

	while (val >= max)
		val -= dif;
	while (val < min)
		val += dif;

	return val;
} 

// Gets the smaller amount of two values
template<typename T>
T Math::Min(T a, T b)
{
	return a < b ? a : b;
}

// Gets the larger amount of two values
template<typename T>
T Math::Max(T a, T b)
{
	return a > b ? a : b;
}

// Clamps a value between a min and max
template<typename T>
T Math::Clamp(T v, T min, T max)
{
	// Ensure min is smaller than max
	if (min > max)
	{
		T t = max;
		max = min;
		min = t;
	}

	return v < min ? min : (v > max ? max : v);
}