/* A rotated box */
struct Box
{
	// Make a box primitive from position, size and rotation
	static Box Make(const Vec3& position, const Vec3& size, const Quat& rotation);

	// Returns a transformed version of the input box
	static Box Transform(const Box& box, const Mat4& transform);

	Mat4 mat;
};

/* A box that is always axis aligned */
struct AxisAlignedBox
{
	Vec3 min;
	Vec3 max;
};

/* A perfect round sphere */
struct Sphere
{
	// Returns a transformed version of the input sphere
	static Sphere Transform(const Sphere& sphere, const Mat4& transform);

	Vec3 origin;
	float radius;
};