#pragma once
#include "SwapChain.h"
#include <array>

constexpr auto MaxRenderTargets = 2u;
constexpr auto MaxViewPorts = 2u;

struct ViewPort
{
	uint32_t Width;
	uint32_t Height;
	uint32_t TopLeftX;
	uint32_t TopLeftY;
};

class VContext
{
public:
	VContext();
public:
	HRESULT IASetPrimitiveTopology(VPRIMITIVE_TOPOLOGY Topology)
	{
		IATopology = Topology;
	}
	HRESULT IASetVertexBuffers(uint32_t StartSlot, uint32_t NumBuffers, IVBuffer* const* ppVertexBuffers, const uint32_t* pStrides, const uint32_t* pOffsets)
	{
		std::copy_n(pStrides + StartSlot, NumBuffers, IAVBStrides);
		for (uint32_t i = StartSlot; i < 4; i++)
		{
			if (i < NumBuffers + StartSlot)
			{
				auto r = static_cast<VBuffer*>(ppVertexBuffers[i])->data;
				IAVertexBuffers[i] = std::span<uint8_t>{ r.data() + pOffsets[i], r.size() - pOffsets[i] };
			}
			IAVertexBuffers[i] = std::span<uint8_t>{};
			IAVBStrides[i] = 0;
		}
		return S_OK;
	}
	HRESULT IASetIndexBuffer(IVBuffer* indexBuffer, VFORMAT format, uint32_t offsetBytes)
	{
		auto& r = static_cast<VBuffer*>(indexBuffer)->data;
		IAIndexBuffer = std::span<uint8_t>{ r.data() + offsetBytes, r.size() - offsetBytes };
		IAIndexFormat = format;
		return S_OK;
	}
	HRESULT RSSetViewports(uint32_t numVPs, const ViewPort* _arr_VPs)
	{
		if (numVPs > MaxRenderTargets) return E_INVALIDARG;
		std::copy_n(_arr_VPs, numVPs, RSViewPorts.begin());
		return S_OK;
	}
	HRESULT OMSetRenderTargets(uint32_t numViews, const RenderTargetView* const _arr_RTVs)noxnd
	{
		if (numViews > MaxRenderTargets) return E_INVALIDARG;
		std::copy_n(_arr_RTVs, numViews, OMRenderTargets.begin());
		return S_OK;
	}
	void ClearRenderTarget(RenderTargetView* rtv, DirectX::PackedVector::XMCOLOR col)
	{
		std::fill((DirectX::PackedVector::XMCOLOR*)rtv->Scan0, (DirectX::PackedVector::XMCOLOR*)rtv->Scan0 + size_t(rtv->Width) * rtv->Height, col);
	}
	void Draw();
private:
	std::array<RenderTargetView, MaxRenderTargets> OMRenderTargets;
	std::array<ViewPort, MaxViewPorts> RSViewPorts;
	std::span<uint8_t> IAIndexBuffer;
	VFORMAT IAIndexFormat;
	VPRIMITIVE_TOPOLOGY IATopology;
	std::array<std::span<uint8_t>, 4> IAVertexBuffers;
	std::array<uint32_t, 4> IAVBStrides;
};