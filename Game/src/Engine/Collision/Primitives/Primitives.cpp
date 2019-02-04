#include "Primitives.h"

// Make a box primitive from position, size and rotation
Box Box::Make(const Vec3& position, const Vec3& size, const Quat& rotation)
{
	Box result;
	result.mat = Mat4::TranslateRotateScale(position, rotation, size);

	return result;
}

// Returns a transformed version of the input box
Box Box::Transform(const Box& box, const Mat4& transform)
{
	Box result;
	result.mat = transform * box.mat;

	return result;
}

// Returns a transformed version of the input sphere
Sphere Sphere::Transform(const Sphere& sphere, const Mat4& transform)
{
	Sphere result;
	result.origin = transform * sphere.origin;

	// Get the scaled radius by only transforming a vector of radius length,
	// 		we ignore translation by setting w to 0, so we only get scale and rotation.
	//		Then we get the length of that vector to find out how much it's scaled.
	float scaled_radius = Vec::Length(transform * Vec4(sphere.radius, 0.f, 0.f, 0.f));
	result.radius = scaled_radius;

	return result;
}