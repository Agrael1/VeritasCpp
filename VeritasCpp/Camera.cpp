#include "Camera.h"

namespace dx = DirectX;

template<typename T>
T wrap_angle(T theta)
{
	T x = (T)fmod(theta + (T)dx::XM_PI, (T)dx::XM_2PI);
	if (x < 0)
		x += (T)dx::XM_2PI;
	return x - (T)dx::XM_PI;
}

Camera::Camera() noexcept
{
	Reset();
}

DirectX::XMMATRIX Camera::GetViewMatrix() const noexcept
{
	using namespace dx;
	const dx::XMVECTOR forwardBaseVector = dx::XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
	// apply the camera rotations to a base vector
	const auto lookVector = dx::XMVector3Transform(forwardBaseVector,
		dx::XMMatrixRotationRollPitchYawFromVector(dx::XMLoadFloat2A(&rot))
	);
	// generate camera transform (applied to all objects to arrange them relative
	// to camera position/orientation in world) from cam position and direction
	// camera "top" always faces towards +Y (cannot do a barrel roll)
	const auto camPosition = dx::XMLoadFloat3A(&pos);
	const auto camTarget = camPosition + lookVector;
	return XMMatrixLookAtLH(camPosition, camTarget, XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f));
}
void Camera::Reset() noexcept
{
	pos = { 0.0f, 0.0f, -2.0f };
	rot = { 0.0f, 0.0f };
}
void Camera::Rotate(float dx, float dy) noexcept
{
	rot.y = wrap_angle(rot.y + dx * rotationSpeed);
	rot.x = std::clamp(rot.x + dy * rotationSpeed, 0.995f * -dx::XM_PIDIV2, 0.995f * dx::XM_PIDIV2);
}
void Camera::Translate(DirectX::XMFLOAT3A translation) noexcept
{
	using namespace dx;
	dx::XMStoreFloat3A(&pos, dx::XMLoadFloat3A(&pos) + dx::XMVector3Transform(
		dx::XMLoadFloat3A(&translation),
		dx::XMMatrixRotationRollPitchYawFromVector(dx::XMLoadFloat2A(&rot)) *
		dx::XMMatrixScaling(travelSpeed, travelSpeed, travelSpeed)
	));
}
