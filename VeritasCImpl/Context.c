#define CONTEXT_IMPL
#include "Context.h"

HRESULT __stdcall IASetPrimitiveTopology(VContext* This, VPRIMITIVE_TOPOLOGY Topology)
{
	return InputASetPrimitiveTopology(&This->IAStage, Topology);
}
HRESULT __stdcall IASetVertexBuffers(VContext* This, uint32_t StartSlot, uint32_t NumBuffers, IVBuffer* const* ppVertexBuffers, const uint32_t* pStrides, const uint32_t* pOffsets)
{
	return InputASetVertexBuffers(&This->IAStage, StartSlot, NumBuffers, ppVertexBuffers, pStrides, pOffsets);
}
HRESULT __stdcall IASetIndexBuffer(VContext* This, IVBuffer* indexBuffer, VFORMAT format, uint32_t offsetBytes)
{
	return InputASetIndexBuffer(&This->IAStage, indexBuffer, format, offsetBytes);
}
HRESULT __stdcall IASetInputLayout(VContext* This, IVInputLayout* pInputLayout)
{
	return InputASetInputLayout(&This->IAStage, pInputLayout);
}
HRESULT __stdcall OMSetRenderTargets(VContext* This, uint32_t numViews, const VRTV_DESC* const _arr_RTVs)
{
	if (numViews > MaxRenderTargets) return E_INVALIDARG;
	size_t sz = numViews * sizeof(VRTV_DESC);
	memcpy_s(This->OMRenderTargets, MaxRenderTargets * sizeof(VRTV_DESC), _arr_RTVs, sz);
	return S_OK;
}
HRESULT __stdcall OMSetDepthStencil(VContext* This, const VDSV_DESC* DSV)
{
	This->OMRenderDepth = *DSV;
	return S_OK;
}
HRESULT __stdcall RSSetViewport(VContext* This, uint32_t numVPs, const VVIEWPORT_DESC* in)
{
	This->RSViewPort = *in;
	const float HalfViewportWidth = in->Width * 0.5f;
	const float HalfViewportHeight = in->Height * 0.5f;
	VMStoreFloat4(&This->RSVPScale, VMVectorSet(HalfViewportWidth, -HalfViewportHeight, 1.0f, 0.0f));
	VMStoreFloat4(&This->RSVPOffset, VMVectorSet(HalfViewportWidth, HalfViewportHeight, 1.0f, 0.0f));
	return S_OK;
}
HRESULT __stdcall ClearRenderTarget(VContext* This, VRTV_DESC* rtv, uint32_t col)
{
	uint32_t* start = rtv->Scan0;
	uint32_t* end = start + rtv->Width * rtv->Height;
	while (start != end)
	{
		*start++ = col;
	}
	return S_OK;
}
HRESULT __stdcall ClearDepthStencil(VContext* This, VDSV_DESC* dsv, float value)
{
	float* start = dsv->Scan0;
	float* end = start + dsv->Width * dsv->Height;
	while (start != end)
	{
		*start++ = value;
	}
	return S_OK;
}

HRESULT RuntimeClassInitialize(VContext* This)
{
    return S_OK;
}
void RuntimeClassDestroy(VContext* This)
{

}
VirtualTable(IVContext)
{
	._unknwn = __COM_IUnknown,

		.IASetIndexBuffer = IASetIndexBuffer,
		.IASetInputLayout = IASetInputLayout,
		.IASetPrimitiveTopology = IASetPrimitiveTopology,
		.IASetVertexBuffers = IASetVertexBuffers,

	.OMSetRenderTargets = OMSetRenderTargets,
	.OMSetDepthStencil = OMSetDepthStencil,

	.RSSetViewport = RSSetViewport,

	.ClearDepthStencil = ClearDepthStencil,
	.ClearRenderTarget = ClearRenderTarget,
};
ENDCLASSDESC
