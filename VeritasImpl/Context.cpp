#include "Device.h"
#include "Context.h"

HRESULT __stdcall VContext::IASetPrimitiveTopology(VPRIMITIVE_TOPOLOGY Topology)
{
	//IATopology = Topology;
	return S_OK;
}
HRESULT __stdcall VContext::IASetVertexBuffers(uint32_t StartSlot, uint32_t NumBuffers, IVBuffer* const* ppVertexBuffers, const uint32_t* pStrides, const uint32_t* pOffsets)
{
	return IAStage.SetVertexBuffers(StartSlot, NumBuffers, ppVertexBuffers, pStrides, pOffsets);
}
HRESULT __stdcall VContext::IASetIndexBuffer(IVBuffer* indexBuffer, VFORMAT format, uint32_t offsetBytes)
{
	return IAStage.SetIndexBuffer(indexBuffer, format, offsetBytes);
}
HRESULT __stdcall VContext::IASetInputLayout(IVInputLayout* pInputLayout)
{
	return IAStage.SetInputLayout(pInputLayout);
}

HRESULT __stdcall VContext::VSSetShader(IVShader* pVertexShader)
{
	VSVertexShader = pVertexShader;
	return S_OK;
}
HRESULT __stdcall VContext::VSSetConstantBuffers(uint32_t StartSlot, uint32_t NumBuffers, IVBuffer* const* ppConstantBuffers)
{
	std::span<VBuffer* const> x{ (VBuffer* const*)ppConstantBuffers, NumBuffers };
	std::copy(x.begin(), x.end(), VSConstantBuffers.begin() + StartSlot);
	return S_OK;
}
HRESULT __stdcall VContext::PSSetShader(IVShader* pPixelShader)
{
	PSPixelShader = pPixelShader;
	return S_OK;
}
HRESULT __stdcall VContext::PSSetConstantBuffers(uint32_t StartSlot, uint32_t NumBuffers, IVBuffer* const* ppConstantBuffers)
{
	std::span<VBuffer*const> x{ (VBuffer*const*)ppConstantBuffers, NumBuffers }; 
	std::copy(x.begin(), x.end(), PSConstantBuffers.begin() + StartSlot);
	return S_OK;
}
HRESULT __stdcall VContext::RSSetViewport(uint32_t numVPs, const VVIEWPORT_DESC* in)
{
	RSViewPort = *in;
	const float HalfViewportWidth = RSViewPort.Width * 0.5f;
	const float HalfViewportHeight = RSViewPort.Height * 0.5f;
	RSVPScale = dx::XMVectorSet(HalfViewportWidth, -HalfViewportHeight, 1.0f, 0.0f);
	RSVPOffset = dx::XMVectorSet(HalfViewportWidth, HalfViewportHeight, 1.0f, 0.0f);
	return S_OK;
}
HRESULT __stdcall VContext::OMSetRenderTargets(uint32_t numViews, const VRTV_DESC* const _arr_RTVs)
{
	if (numViews > MaxRenderTargets) return E_INVALIDARG;
	std::copy_n(_arr_RTVs, numViews, OMRenderTargets.begin());
	return S_OK;
}
HRESULT __stdcall VContext::OMSetDepthStencil(const VDSV_DESC* DSV)
{
	if (DSV)
		OMRenderDepth = *DSV;
	else
		OMRenderDepth.Scan0 = nullptr;
	return S_OK;
}
HRESULT __stdcall VContext::ClearRenderTarget(VRTV_DESC* rtv, uint32_t col)
{
	std::fill((DirectX::PackedVector::XMCOLOR*)rtv->Scan0, (DirectX::PackedVector::XMCOLOR*)rtv->Scan0 + size_t(rtv->Width) * rtv->Height, col);
	return S_OK;
}

HRESULT __stdcall VContext::Map(IVBuffer* pResource, VMAPPED_SUBRESOURCE* _out_pMappedResource)
{
	auto* x = dynamic_cast<VBuffer*>(pResource);
	if (!x) return E_POINTER;

	_out_pMappedResource->pData = x->data.data();
	_out_pMappedResource->RowPitch = x->desc.StructureByteStride;
	_out_pMappedResource->DepthPitch = x->desc.ByteWidth;

	return S_OK;
}

HRESULT __stdcall VContext::Unmap(IVBuffer* pResource)
{
	// may be used for multithreading for unlocking buffer for gpu thread 
	return E_NOTIMPL;
}

void __stdcall VContext::DrawIndexed(uint32_t nofVertices)
{
	auto verts = IAStage.MakeVerticesIndexed(nofVertices);

	std::array<void*, 4> x{};
	for (size_t i = 0; i<4; i++)
	{
		x[i] = VSConstantBuffers[i] ? VSConstantBuffers[i]->data.data() : nullptr;
	}
	VSVertexShader->UpdateConstants(x.data());

	for (size_t i = 0; i < 4; i++)
	{
		x[i] = PSConstantBuffers[i] ? PSConstantBuffers[i]->data.data() : nullptr;
	}
	PSPixelShader->UpdateConstants(x.data());

	std::vector<XMVSOut> VSOut;
	VSOut.resize(verts.size());

	for (uint32_t i = 0; auto & v : verts)
	{
		VSVertexShader->Invoke(&v, &VSOut[i]);
	}
	AssembleTriangles(VSOut);
}


HRESULT InputAssembler::SetIndexBuffer(IVBuffer* indexBuffer, VFORMAT format, uint32_t offsetBytes)
{
	IAIndexFormat = format;
	if (format)
	{
		IAIndexBuffer = dynamic_cast<VBuffer*>(indexBuffer);
		IAIndexOffset = offsetBytes;
	}
	else
	{
		IAIndexBuffer = nullptr;
	}
	return S_OK;
}
HRESULT InputAssembler::SetVertexBuffers(uint32_t StartSlot, uint32_t NumBuffers, IVBuffer* const* ppVertexBuffers, const uint32_t* pStrides, const uint32_t* pOffsets)
{
	if (StartSlot + NumBuffers > 4 || !ppVertexBuffers) return E_INVALIDARG;
	std::copy_n(pStrides + StartSlot, NumBuffers, IABufferStrides.begin());
	std::copy_n(pOffsets + StartSlot, NumBuffers, IABufferOffsets.begin());

	for (uint32_t i = StartSlot; i < StartSlot + NumBuffers; i++)
	{
		IAVertexBuffers[i] = dynamic_cast<VBuffer*>(ppVertexBuffers[i]);
	}
	return S_OK;
}
HRESULT InputAssembler::SetInputLayout(IVInputLayout* pInputLayout)
{
	IAInputLayout = dynamic_cast<VInputLayout*>(pInputLayout);
	return S_OK;
}
