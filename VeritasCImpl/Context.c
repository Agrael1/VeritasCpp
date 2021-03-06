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

HRESULT __stdcall VSSetShader(VContext* This, IVShader* pVertexShader)
{
	ComPtrRelease(This->VSVertexShader);
	
	This->VSVertexShader = pVertexShader;
	if (This->VSVertexShader)
	{
		This->VSVertexShader->method->_unknwn.AddRef((IUnknown*)This->VSVertexShader);
	}
	return S_OK;
}
HRESULT __stdcall PSSetShader(VContext* This, IVShader* pPixelShader)
{
	return RasterSetShader(&This->RSStage, pPixelShader);
}

HRESULT __stdcall VSSetConstantBuffers(VContext* This, uint32_t StartSlot, uint32_t NumBuffers, IVBuffer* const* ppConstantBuffers)
{
	if (StartSlot + NumBuffers > MaxBuffers) return E_BOUNDS;
	VBuffer** Start = This->VSConstantBuffers + StartSlot;
	VBuffer** End = Start + NumBuffers;

	while (Start != End)
	{
		IUnknown** Buffer = (IUnknown**)Start;
		ComPtrRelease(*Buffer);

		*Start++ = (VBuffer*)*ppConstantBuffers++;
		if ((*Buffer))
		{
			(*Buffer)->method->AddRef(*Buffer);
		}
	}
	return S_OK;
}
HRESULT __stdcall PSSetConstantBuffers(VContext* This, uint32_t StartSlot, uint32_t NumBuffers, IVBuffer* const* ppConstantBuffers)
{
	return RasterSetConstantBuffers(&This->RSStage, StartSlot, NumBuffers, ppConstantBuffers);
}

HRESULT __stdcall OMSetRenderTargets(VContext* This, uint32_t numViews, const VRTV_DESC* const _arr_RTVs)
{
	if (numViews > MaxRenderTargets) return E_INVALIDARG;
	size_t sz = numViews * sizeof(VRTV_DESC);
	memcpy_s(This->RSStage.OMRenderTargets, MaxRenderTargets * sizeof(VRTV_DESC), _arr_RTVs, sz);
	return S_OK;
}
HRESULT __stdcall OMSetDepthStencil(VContext* This, const VDSV_DESC* DSV)
{
	This->RSStage.OMRenderDepth = *DSV;
	return S_OK;
}
HRESULT __stdcall RSSetViewport(VContext* This, uint32_t numVPs, const VVIEWPORT_DESC* in)
{
	return RasterSetViewPort(&This->RSStage, numVPs, in);
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
HRESULT __stdcall Map(VContext* This, IVBuffer* pResource, VMAPPED_SUBRESOURCE* _out_pMappedResource)
{
	VBuffer* x = (VBuffer*)(pResource);
	if (!x) return E_POINTER;

	_out_pMappedResource->pData = x->data;
	_out_pMappedResource->RowPitch = x->desc.StructureByteStride;
	_out_pMappedResource->DepthPitch = x->desc.ByteWidth;

	return S_OK;
}
HRESULT __stdcall Unmap(VContext* This, IVBuffer* pResource)
{
	return E_NOTIMPL;
}

HRESULT __stdcall DrawIndexed(VContext* This, uint32_t nofVertices)
{
	if (!This->VSVertexShader) return E_PENDING;
	void* x[MaxBuffers] = {NULL};
	for (size_t i = 0; i < 4; i++)
	{
		x[i] = This->VSConstantBuffers[i] ? This->VSConstantBuffers[i]->data : nullptr;
	}
	This->VSVertexShader->method->UpdateConstants(This->VSVertexShader, x);

	if (This->RSStage.PSPixelShader)
	{
		for (size_t i = 0; i < 4; i++)
		{
			x[i] = This->RSStage.PSConstantBuffers[i] ? This->RSStage.PSConstantBuffers[i]->data : nullptr;
		}
		This->RSStage.PSPixelShader->method->UpdateConstants(This->RSStage.PSPixelShader, x);
	}


	VMVertex* verts = _mm_malloc(nofVertices * sizeof(VMVertex), _Alignof(VMVertex));
	InputAMakeVerticesIndexed(&This->IAStage, nofVertices, verts);

	for (uint32_t i = 0; i < nofVertices; i++)
	{
		This->VSVertexShader->method->Invoke(This->VSVertexShader, verts + i, verts + i);
	}

	ShaderPrivateData pd;
	This->VSVertexShader->method->GetShaderPrivateData(This->VSVertexShader, &pd);

	for (size_t it = 0u; it < nofVertices; it += 3)
	{
		RasterizeTriangle(&This->RSStage, verts + it, pd);
	}

	_mm_free(verts);
}

HRESULT RuntimeClassInitialize(VContext* This)
{
    return S_OK;
}
void RuntimeClassDestroy(VContext* This)
{
	InputADestroy(&This->IAStage);
	ComPtrRelease(This->RSStage.PSPixelShader);
	ComPtrRelease(This->VSVertexShader);
	for (uint32_t i = 0; i < MaxBuffers; i++)
	{
		ComPtrRelease(This->RSStage.PSConstantBuffers[i]);
		ComPtrRelease(This->VSConstantBuffers[i]);
	}

}
VirtualTable(IVContext)
{
	._unknwn = __COM_IUnknown,

	.IASetIndexBuffer = IASetIndexBuffer,
	.IASetInputLayout = IASetInputLayout,
	.IASetPrimitiveTopology = IASetPrimitiveTopology,
	.IASetVertexBuffers = IASetVertexBuffers,

	.VSSetConstantBuffers = VSSetConstantBuffers,
	.PSSetConstantBuffers = PSSetConstantBuffers,

	.PSSetShader = PSSetShader,
	.VSSetShader = VSSetShader,

	.OMSetRenderTargets = OMSetRenderTargets,
	.OMSetDepthStencil = OMSetDepthStencil,

	.RSSetViewport = RSSetViewport,

	.ClearDepthStencil = ClearDepthStencil,
	.ClearRenderTarget = ClearRenderTarget,
	.Map = Map,
	.Unmap = Unmap,

	.DrawIndexed = DrawIndexed
};
ENDCLASSDESC
