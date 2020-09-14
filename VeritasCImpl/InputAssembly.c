#include "InputAssembly.h"

HRESULT __stdcall InputASetPrimitiveTopology(InputAssembler* This, VPRIMITIVE_TOPOLOGY Topology)
{
	return E_NOTIMPL;
}


HRESULT __stdcall InputASetVertexBuffers(InputAssembler* This, uint32_t StartSlot, uint32_t NumBuffers, IVBuffer* const* ppVertexBuffers, const uint32_t* pStrides, const uint32_t* pOffsets)
{
	if (StartSlot + NumBuffers > MaxBuffers) return E_BOUNDS;
	VBuffer** Start = This->IAVertexBuffers + StartSlot;
	VBuffer** End = Start + NumBuffers;

	uint32_t* StrStart = This->IABufferStrides + StartSlot;
	uint32_t* OfsStart = This->IABufferOffsets + StartSlot;

	while (Start != End)
	{
		IUnknown** Buffer = (IUnknown**)Start;
		if (*Buffer)
		{
			(*Buffer)->method->Release(*Buffer);
		}
		*Start++ = (VBuffer*)*ppVertexBuffers++;
		if((*Buffer))
			(*Buffer)->method->AddRef(*Buffer);

		*StrStart++ = *pStrides++;
		*OfsStart++ = *pOffsets++;
	}
	return S_OK;
}
HRESULT __stdcall InputASetIndexBuffer(InputAssembler* This, IVBuffer* indexBuffer, VFORMAT format, uint32_t offsetBytes)
{
	if (This->IAIndexBuffer)
	{
		((IUnknown*)This->IAIndexBuffer)->method->Release((IUnknown*)This->IAIndexBuffer);
	}
	This->IAIndexBuffer = (VBuffer*)indexBuffer;
	if(indexBuffer)
		indexBuffer->method->_unknwn.AddRef((IUnknown*)indexBuffer);

	This->IAIndexFormat = format;
	This->IAIndexOffset = offsetBytes;
	return S_OK;
}

HRESULT __stdcall InputASetInputLayout(InputAssembler* This, IVInputLayout* pInputLayout)
{
	if (This->IAInputLayout)
	{
		((IUnknown*)This->IAInputLayout)->method->Release((IUnknown*)This->IAInputLayout);
	}
	This->IAInputLayout = (VInputLayout*)pInputLayout;
	if (pInputLayout)
		pInputLayout->method->_unknwn.AddRef((IUnknown*)pInputLayout);
	return S_OK;
}
