#pragma once
#include <DirectXMath.h>

union alignas(16) vec3A
{
	vec3A() = default;
	vec3A(float _x, float _y, float _z) : x(_x), y(_y), z(_z) {};
	vec3A(DirectX::XMVECTOR _v) : v(_v) {};
	struct alignas(16)
	{
		float x;
		float y;
		float z;
	};
	DirectX::XMVECTOR v;

	operator DirectX::XMVECTOR()const
	{
		return this->v;
	}
};

union alignas(16) vec2A
{
	vec2A() = default;
	vec2A(union vec3A vec) : v(vec.v) {};
	vec2A(float _x, float _y) : x(_x), y(_y) {};
	vec2A(DirectX::XMVECTOR _v) : v(_v) {};
	struct alignas(16)
	{
		float x;
		float y;
	};
	DirectX::XMVECTOR v;

	operator DirectX::XMVECTOR()const
	{
		return this->v;
	}
};



template<typename T>
T wrap_angle(T theta)
{
	using namespace DirectX;
	const T modded = fmod(theta, (T)XM_2PI);
	return (modded > (T)XM_PI) ?
		(modded - (T)XM_2PI) :
		modded;
}

inline DirectX::XMMATRIX RotationZ(float theta)
{
	const float sinTheta = sin(theta);
	const float cosTheta = cos(theta);
	return{
		 cosTheta, sinTheta, (float)0.0,(float)0.0,
		-sinTheta, cosTheta, (float)0.0,(float)0.0,
		(float)0.0,    (float)0.0,   (float)1.0,(float)0.0,
		(float)0.0,(float)0.0,(float)0.0,(float)1.0
	};
}
inline DirectX::XMMATRIX RotationY(float theta)
{
	const float sinTheta = sin(theta);
	const float cosTheta = cos(theta);
	return{
		 cosTheta, (float)0.0,-sinTheta,(float)0.0,
		 (float)0.0,   (float)1.0, (float)0.0,(float)0.0,
		 sinTheta, (float)0.0, cosTheta,(float)0.0,
		 (float)0.0,(float)0.0,(float)0.0,(float)1.0
	};
}
inline DirectX::XMMATRIX RotationX(float theta)
{
	const float sinTheta = sin(theta);
	const float cosTheta = cos(theta);
	return{
		(float)1.0, (float)0.0,   (float)0.0,(float)0.0,
		(float)0.0, cosTheta, sinTheta,(float)0.0,
		(float)0.0,-sinTheta, cosTheta,(float)0.0,
		(float)0.0,(float)0.0,(float)0.0,(float)1.0
	};
}
