#pragma once
#include <VeritasMath.h>

class Camera
{
public:
	Camera()noexcept;
public:
	DirectX::XMMATRIX GetViewMatrix()const noexcept;
	void Reset()noexcept;
	void Rotate(float dx, float dy)noexcept;
	void Translate(float3 translation)noexcept;
private:
	float3 pos;
	float2 rot;
	static constexpr const float travelSpeed = 12.0f;
	static constexpr const float rotationSpeed = 0.004f;
};