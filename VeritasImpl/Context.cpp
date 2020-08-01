#include "Device.h"
#include "Context.h"

HRESULT __stdcall VContext::IASetPrimitiveTopology(VPRIMITIVE_TOPOLOGY Topology)
{
	IATopology = Topology;
	return S_OK;
}
HRESULT __stdcall VContext::IASetVertexBuffers(uint32_t StartSlot, uint32_t NumBuffers, IVBuffer* const* ppVertexBuffers, const uint32_t* pStrides, const uint32_t* pOffsets)
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
HRESULT __stdcall VContext::IASetIndexBuffer(IVBuffer* indexBuffer, VFORMAT format, uint32_t offsetBytes)
{
	auto& r = static_cast<VBuffer*>(indexBuffer)->data;
	IAIndexBuffer = std::span<uint8_t>{ r.data() + offsetBytes, r.size() - offsetBytes };
	IAIndexFormat = format;
	return S_OK;
}
HRESULT __stdcall VContext::IASetInputLayout(IVInputLayout* pInputLayout)
{
	IAInputLayout = static_cast<VInputLayout*>(pInputLayout);
	return S_OK;
}
HRESULT __stdcall VContext::RSSetViewports(uint32_t numVPs, const VVIEWPORT_DESC* _arr_VPs)
{
	if (numVPs > MaxRenderTargets) return E_INVALIDARG;
	std::copy_n(_arr_VPs, numVPs, RSViewPorts.begin());
	return S_OK;
}
HRESULT __stdcall VContext::OMSetRenderTargets(uint32_t numViews, const VRTV_DESC* const _arr_RTVs)
{
	if (numViews > MaxRenderTargets) return E_INVALIDARG;
	std::copy_n(_arr_RTVs, numViews, OMRenderTargets.begin());
	return S_OK;
}
HRESULT __stdcall VContext::ClearRenderTarget(VRTV_DESC* rtv, uint32_t col)
{
	std::fill((DirectX::PackedVector::XMCOLOR*)rtv->Scan0, (DirectX::PackedVector::XMCOLOR*)rtv->Scan0 + size_t(rtv->Width) * rtv->Height, col);
	return S_OK;
}


