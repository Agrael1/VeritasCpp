#pragma once
#include <Framework/DirectXMath/Inc/DirectXPackedVector.h>

namespace dx = DirectX;
using matrix = dx::XMMATRIX;
using SV_VertexID = uint32_t;

class VMVECTOR
{
public:
	VMVECTOR() = default;
	VMVECTOR(dx::XMVECTOR in)noexcept
	{
		v = in;
	}
public:
	friend dx::XMVECTOR operator/(VMVECTOR V, float scalar);
	friend dx::XMVECTOR operator*(VMVECTOR V, float scalar);
	operator dx::XMVECTOR()const
	{
		return v;
	}
private:
	dx::XMVECTOR v;
};

struct float1
{
	float1() = default;
	float1(const dx::XMVECTOR in)
	{
		dx::XMStoreFloat(&x, in);
	}
	float1(const float in)
	{
		x = in;
	}
	float1 operator=(const dx::XMVECTOR in)
	{
		dx::XMStoreFloat(&x, in);
		return *this;
	}
	operator dx::XMVECTOR()const
	{
		return dx::XMLoadFloat(&x);
	}
	operator float()const
	{
		return x;
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
	float2() = default;
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
	float3() = default;
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
	float4() = default;
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
};
struct alignas(16) float2A : public dx::XMFLOAT2A
{
	float2A() = default;
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
};
struct alignas(16) float3A : public dx::XMFLOAT3A
{
	float3A() = default;
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
	operator float3()const
	{
		return (float3&)*this;
	}
};
struct alignas(16) float4A : public dx::XMFLOAT4A
{
	float4A() = default;
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
};


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