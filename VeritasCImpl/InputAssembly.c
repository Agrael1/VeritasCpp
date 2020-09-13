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
	while (Start < End)
	{
		IUnknown* Buffer = (IUnknown*)*Start;
		if (Buffer)
		{
			Buffer->method->Release(Buffer);
		}
	}
	return S_OK;
}
