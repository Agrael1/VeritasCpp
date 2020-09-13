#define CONTEXT_IMPL
#include "Context.h"

//HRESULT __stdcall IASetPrimitiveTopology(VContext* This, VPRIMITIVE_TOPOLOGY Topology)
//{
//	return E_NOTIMPL;
//}
//HRESULT __stdcall IASetVertexBuffers(VContext* This, uint32_t StartSlot, uint32_t NumBuffers, IVBuffer* const* ppVertexBuffers, const uint32_t* pStrides, const uint32_t* pOffsets)
//{
//
//	This->IAStage.IAVertexBuffers
//}
//HRESULT __stdcall IASetIndexBuffer(IVBuffer* indexBuffer, VFORMAT format, uint32_t offsetBytes)override;
//HRESULT __stdcall IASetInputLayout(IVInputLayout* pInputLayout)override;
HRESULT __stdcall OMSetRenderTargets(VContext* This, uint32_t numViews, const VRTV_DESC* const _arr_RTVs)
{
	if (numViews > MaxRenderTargets) return E_INVALIDARG;
	size_t sz = numViews * sizeof(VRTV_DESC);
	memcpy_s(This->OMRenderTargets, MaxRenderTargets * sizeof(VRTV_DESC), _arr_RTVs, sz);
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
	.OMSetRenderTargets = OMSetRenderTargets,
	.RSSetViewport = RSSetViewport,
};
ENDCLASSDESC
