#pragma once
#include <array>
#include <cassert>
#include <Framework\DirectXMath\Inc\DirectXMath.h>

struct Plane
{
	static constexpr std::array<DirectX::XMFLOAT3A, 4> Vertices()
	{
		return std::array<DirectX::XMFLOAT3A, 4>
		{
			DirectX::XMFLOAT3A{ 0.5f, 0.5f, 0.5f },
				DirectX::XMFLOAT3A{ -0.5f, 0.5f, 0.5f },
				DirectX::XMFLOAT3A{ 0.5f, -0.5f, 0.5f },
				DirectX::XMFLOAT3A{ -0.5f, -0.5f, 0.5f },
		};
	}
	static constexpr std::array<unsigned, 6> Indices()
	{
		return std::array<unsigned, 6>
		{
			1,2,3, 2,0,3
		};
	}
};