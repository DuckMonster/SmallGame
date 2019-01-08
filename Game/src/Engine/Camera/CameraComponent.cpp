
#include "CameraComponent.h"
#include "Core/Class/TypeHelpers.h"

REGISTER_TYPE(CameraComponent);
REGISTER_PROP(CameraComponent, position);
REGISTER_PROP(CameraComponent, forward);
REGISTER_PROP(CameraComponent, up);

void CameraComponent::GetMatrix(Mat4& matrix)
{
	Mat4 lookAt = Mat4::LookAt(position, position + forward, up);

	float aspect = (float)gContext->width / (float)gContext->height;
	//Mat4 projection = Mat4::Ortho(-5.f * aspect, 5.f * aspect, -5.f, 5.f, -20.f, 20.f);
	Mat4 projection = Mat4::Perspective(Math::Radians(45.f), aspect, 1.f, 100.f);
	Vec4 test = projection * Vec4(0.f, 0.f, 10.f, 1.f);
	test /= test.w;

	Vec4 result = projection * lookAt * Vec4(0.f, 0.f, 0.f, 1.f);

	matrix = projection * lookAt;
}

void CameraComponent::GetMatrixInv(Mat4& matrix)
{
	GetMatrix(matrix);
	matrix = Mat4::Inverse(matrix);
}
