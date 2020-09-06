#pragma once
#include "DirectXMath\Inc\DirectXPackedVector.h"

namespace dx = DirectX;
using SV_VertexID = uint32_t;

class VMVECTOR
{
public:
	VMVECTOR() = default;
	constexpr VMVECTOR(dx::XMVECTOR in)noexcept : v(in){}
public:
	friend dx::XMVECTOR operator/(VMVECTOR V, float scalar);
	friend dx::XMVECTOR operator*(VMVECTOR V, float scalar);
	constexpr operator dx::XMVECTOR()const
	{
		return v;
	}
private:
	dx::XMVECTOR v;
};

struct float1
{
	constexpr float1() :x(0) {};
	constexpr float1(const float in): x(in){}

	float1(const dx::XMVECTOR in)
	{
		dx::XMStoreFloat(&x, in);
	}
	float1 operator=(const dx::XMVECTOR in)
	{
		dx::XMStoreFloat(&x, in);
		return *this;
	}
	constexpr operator float()const
	{
		return x;
	}
	operator dx::XMVECTOR()const
	{
		return dx::XMLoadFloat(&x);
	}
	operator VMVECTOR()const
	{
		return dx::XMLoadFloat(&x);
	}
public:
	float x;
};
struct float2 : public dx::XMFLOAT2
{
	using dx::XMFLOAT2::XMFLOAT2;

	constexpr float2() :dx::XMFLOAT2(0, 0) {}
	constexpr float2(const dx::XMFLOAT2 in) : dx::XMFLOAT2(in) {}
	float2(const dx::XMVECTOR in)
	{
		dx::XMStoreFloat2(this, in);
	}
	float2 operator=(const dx::XMVECTOR in)
	{
		dx::XMStoreFloat2(this, in);
		return *this;
	}
	operator dx::XMVECTOR()const
	{
		return dx::XMLoadFloat2(this);
	}
	operator VMVECTOR()const
	{
		return dx::XMLoadFloat2(this);
	}
};
struct float3 : public dx::XMFLOAT3
{
	using dx::XMFLOAT3::XMFLOAT3;

	constexpr float3() :dx::XMFLOAT3(0, 0, 0) {}
	constexpr float3(const dx::XMFLOAT3 in) : dx::XMFLOAT3(in) {}
	float3(const dx::XMVECTOR in)
	{
		dx::XMStoreFloat3(this, in);
	}
	float3 operator=(const dx::XMVECTOR in)
	{
		dx::XMStoreFloat3(this, in);
		return *this;
	}
	operator dx::XMVECTOR()const
	{
		return dx::XMLoadFloat3(this);
	}
	operator VMVECTOR()const
	{
		return dx::XMLoadFloat3(this);
	}
};
struct float4 : public dx::XMFLOAT4
{
	using dx::XMFLOAT4::XMFLOAT4;

	constexpr float4() :dx::XMFLOAT4(0, 0, 0, 0) {}
	constexpr float4(const dx::XMFLOAT4 in) : dx::XMFLOAT4(in){}
	float4(const dx::XMVECTOR in)
	{
		dx::XMStoreFloat4(this, in);
	}
	float4 operator=(const dx::XMVECTOR in)
	{
		dx::XMStoreFloat4(this, in);
		return *this;
	}
	operator dx::XMVECTOR()const
	{
		return dx::XMLoadFloat4(this);
	}
	operator VMVECTOR()const
	{
		return dx::XMLoadFloat4(this);
	}
};

struct alignas(16) float1A : public float1
{
	using float1::float1;
};
struct alignas(16) float2A : public dx::XMFLOAT2A
{
	using dx::XMFLOAT2A::XMFLOAT2A;

	constexpr float2A() :dx::XMFLOAT2A(0, 0) {}
	float2A(const dx::XMVECTOR in)
	{
		dx::XMStoreFloat2A(this, in);
	}
	float2A operator=(const dx::XMVECTOR in)
	{
		dx::XMStoreFloat2A(this, in);
		return *this;
	}
	operator dx::XMVECTOR()const
	{
		return dx::XMLoadFloat2A(this);
	}
	operator VMVECTOR()const
	{
		return dx::XMLoadFloat2A(this);
	}
	constexpr operator float2()const
	{
		return (dx::XMFLOAT2)*this;
	}
};
struct alignas(16) float3A : public dx::XMFLOAT3A
{
	using dx::XMFLOAT3A::XMFLOAT3A;

	constexpr float3A() :dx::XMFLOAT3A(0, 0, 0) {}
	float3A(const dx::XMVECTOR in)
	{
		dx::XMStoreFloat3A(this, in);
	}
	float3A operator=(const dx::XMVECTOR in)
	{
		dx::XMStoreFloat3A(this, in);
		return *this;
	}
	operator dx::XMVECTOR()const
	{
		return dx::XMLoadFloat3A(this);
	}
	operator VMVECTOR()const
	{
		return dx::XMLoadFloat3A(this);
	}
	constexpr operator float3()const
	{
		return (dx::XMFLOAT3)*this;
	}
};
struct alignas(16) float4A : public dx::XMFLOAT4A
{
	using dx::XMFLOAT4A::XMFLOAT4A;

	constexpr float4A() :dx::XMFLOAT4A(0, 0, 0, 0) {}
	float4A(const dx::XMVECTOR in)
	{
		dx::XMStoreFloat4A(this, in);
	}
	float4A operator=(const dx::XMVECTOR in)
	{
		dx::XMStoreFloat4A(this, in);
		return *this;
	}
	operator dx::XMVECTOR()const
	{
		return dx::XMLoadFloat4A(this);
	}
	operator VMVECTOR()const
	{
		return dx::XMLoadFloat4A(this);
	}
	constexpr operator float4()const
	{
		return (dx::XMFLOAT4)*this;
	}
};

struct float4x4 : public dx::XMFLOAT4X4
{
	using dx::XMFLOAT4X4::XMFLOAT4X4;
	constexpr float4x4() : dx::XMFLOAT4X4(1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f)
	{}
	float4x4(const dx::XMMATRIX in)
	{
		dx::XMStoreFloat4x4(this, in);
	}
	float4x4& operator=(const dx::XMMATRIX in)
	{
		dx::XMStoreFloat4x4(this, in);
		return *this;
	}
	operator dx::XMMATRIX()const
	{
		return dx::XMLoadFloat4x4(this);
	}
};

using matrix = float4x4;

#if (defined(__clang__) || defined(__GNUC__))
inline dx::XMVECTOR operator/(VMVECTOR V, float scalar)
{
	dx::XMVECTOR vS = dx::XMVectorReplicate(scalar);
	return dx::XMVectorDivide(V.v, vS);
}
inline dx::XMVECTOR operator*(VMVECTOR V, float scalar)
{
	dx::XMVECTOR vS = dx::XMVectorReplicate(scalar);
	return dx::XMVectorMultiply(V.v, vS);
}
inline dx::XMVECTOR operator*(float scalar, VMVECTOR V)
{
	return V * scalar;
}
#endif