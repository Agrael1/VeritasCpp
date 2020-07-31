#pragma once
#include <Interfaces.h>
#include <array>
#include <span>
#include <algorithm>

#include "Buffer.h"
#include "InputLayout.h"
#include "Texture.h"

#include <DirectXMath\Inc\DirectXPackedVector.h>


class VContext: public wrl::RuntimeClass<wrl::RuntimeClassFlags<wrl::ClassicCom>, IVContext>
{
public:
	VContext();
public:
	HRESULT __stdcall IASetPrimitiveTopology(VPRIMITIVE_TOPOLOGY Topology)override
	{
		IATopology = Topology;
	}
	HRESULT __stdcall IASetVertexBuffers(uint32_t StartSlot, uint32_t NumBuffers, IVBuffer* const* ppVertexBuffers, const uint32_t* pStrides, const uint32_t* pOffsets)override
	{
		std::copy_n(pStrides + StartSlot, NumBuffers, IAVBStrides.begin());
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
	HRESULT __stdcall IASetIndexBuffer(IVBuffer* indexBuffer, VFORMAT format, uint32_t offsetBytes)override
	{
		auto& r = static_cast<VBuffer*>(indexBuffer)->data;
		IAIndexBuffer = std::span<uint8_t>{ r.data() + offsetBytes, r.size() - offsetBytes };
		IAIndexFormat = format;
		return S_OK;
	}
	HRESULT __stdcall IASetInputLayout(IVInputLayout* pInputLayout)override
	{
		IAInputLayout = static_cast<VInputLayout*>(pInputLayout);
		return S_OK;
	}
	HRESULT __stdcall RSSetViewports(uint32_t numVPs, const VVIEWPORT_DESC* _arr_VPs)override
	{
		if (numVPs > MaxRenderTargets) return E_INVALIDARG;
		std::copy_n(_arr_VPs, numVPs, RSViewPorts.begin());
		return S_OK;
	}
	HRESULT __stdcall OMSetRenderTargets(uint32_t numViews, const VRTV_DESC* const _arr_RTVs)override
	{
		if (numViews > MaxRenderTargets) return E_INVALIDARG;
		std::copy_n(_arr_RTVs, numViews, OMRenderTargets.begin());
		return S_OK;
	}
	HRESULT __stdcall ClearRenderTarget(VRTV_DESC* rtv, uint32_t col)override
	{
		std::fill((DirectX::PackedVector::XMCOLOR*)rtv->Scan0, (DirectX::PackedVector::XMCOLOR*)rtv->Scan0 + size_t(rtv->Width) * rtv->Height, col);
		return S_OK;
	}
private:
	std::array<VRTV_DESC, MaxRenderTargets> OMRenderTargets;
	std::array<VVIEWPORT_DESC, MaxViewPorts> RSViewPorts;
	std::span<uint8_t> IAIndexBuffer;
	VFORMAT IAIndexFormat;
	VPRIMITIVE_TOPOLOGY IATopology;
	VInputLayout* IAInputLayout;
	std::array<std::span<uint8_t>, 4> IAVertexBuffers;
	std::array<uint32_t, 4> IAVBStrides;
};

