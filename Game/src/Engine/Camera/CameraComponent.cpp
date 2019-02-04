
#include "CameraComponent.h"
#include "Core/Class/TypeHelpers.h"

REGISTER_TYPE(CameraComponent);
REGISTER_PROP(CameraComponent, position);
REGISTER_PROP(CameraComponent, forward);
REGISTER_PROP(CameraComponent, up);

void CameraComponent::GetMatrix(Mat4& matrix)
{
	Mat4 look_at = Mat4::LookAt(position, position + forward, up);

	float aspect = (float)gContext->width / (float)gContext->height;
	Mat4 projection = Mat4::Ortho(-10.f * aspect, 10.f * aspect, -10.f, 10.f, -50.f, 50.f);
	Vec4 test = projection * Vec4(0.f, 0.f, 10.f, 1.f);
	test /= test.w;

	Vec4 result = projection * look_at * Vec4(0.f, 0.f, 0.f, 1.f);

	matrix = projection * look_at;
}

void CameraComponent::GetMatrixInv(Mat4& matrix)
{
	GetMatrix(matrix);
	matrix = Mat4::Inverse(matrix);
}
