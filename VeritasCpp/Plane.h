#pragma once
#include <array>
#include <cassert>
#include <Framework\DirectXMath\Inc\DirectXMath.h>

struct Plane
{
	struct PVtx
	{
		DirectX::XMFLOAT3 pos;
		DirectX::XMFLOAT3 normal;
	};
	static constexpr std::array<PVtx, 4> Vertices()
	{
		return std::array<PVtx, 4>
		{
			PVtx{ DirectX::XMFLOAT3{ 0.5f, 0.5f, 0.5f }, DirectX::XMFLOAT3{ 0.0f, 0.0f, -1.0f } },
			PVtx{ DirectX::XMFLOAT3{ -0.5f, 0.5f, 0.5f },DirectX::XMFLOAT3{ 0.0f, 0.0f, -1.0f } },
			PVtx{ DirectX::XMFLOAT3{ 0.5f, -0.5f, 0.5f },DirectX::XMFLOAT3{ 0.0f, 0.0f, -1.0f } },
			PVtx{ DirectX::XMFLOAT3{ -0.5f, -0.5f, 0.5f},DirectX::XMFLOAT3{ 0.0f, 0.0f, -1.0f } },
		};
	}
	static constexpr std::array<unsigned, 6> Indices()
	{
		return std::array<unsigned, 6>
		{
			1,2,3, 1,0,2
		};
	}
};