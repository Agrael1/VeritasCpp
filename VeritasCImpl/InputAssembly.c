#include "InputAssembly.h"
#include <limits.h>

extern inline void ComPtrRelease(IUnknown* This);

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
        ComPtrRelease(*Buffer);

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

uint32_t InputAGetMonotonicSize(const InputAssembler* This)
{
    return (uint32_t)This->IAInputLayout->monotonicSize;
}
void InputAMakeVertices(const InputAssembler* This, uint32_t NumVertices, VMVertex* _out_VArray)
{
    assert(NumVertices % 3 == 0);
    VMVertex* out = _out_VArray;

    VMVertex* OutStart = out;
    VMVertex* OutEnd = OutStart + NumVertices;
    uint32_t MSize = InputAGetMonotonicSize(This);
    unsigned vtx = 0;

    while (OutStart != OutEnd)
    {
        for (uint32_t i = 0; i < MSize; i++)
        {
            VINPUT_ELEMENT* x = &This->IAInputLayout->il[i];
            size_t index = vtx * This->IABufferStrides[x->InputSlot] + This->IABufferOffsets[x->InputSlot] + x->AlignedByteOffset;
            uint8_t* data = &This->IAVertexBuffers[x->InputSlot]->data[index];

            switch (x->Format)
            {
            case FORMAT_R32G32B32A32_FLOAT:
                (*OutStart).data[i].v = VMLoadFloat4((VMFLOAT4*)(data));
                break;
            case FORMAT_R32G32B32_FLOAT:
                (*OutStart).data[i].v = VMLoadFloat3((VMFLOAT3*)(data));
                break;
            case FORMAT_R32G32_FLOAT:
                (*OutStart).data[i].v = VMLoadFloat2((VMFLOAT2*)(data));
                break;
            case FORMAT_R32_FLOAT:
                (*OutStart).data[i].v = VMLoadFloat((float*)(data));
                break;
            default:
                break;
            }
        }
        (*OutStart).SV_VertexID = vtx++;
    }
}
void InputAMakeVerticesIndexed(const InputAssembler* This, uint32_t NumIndices, VMVertex* _out_VArray)
{
    assert(NumIndices % 3 == 0);
    assert(This->IAIndexBuffer);
	VMVertex* out = _out_VArray;
	uint8_t* indexPtr = This->IAIndexBuffer->data;

	VMVertex* OutStart = out;
	VMVertex* OutEnd = OutStart + NumIndices;
	uint32_t MSize = InputAGetMonotonicSize(This);

    while (OutStart != OutEnd)
    {
        (*OutStart).SV_VertexID = (*((uint32_t*)indexPtr)) & ((1ull << This->IAIndexFormat * CHAR_BIT) - 1);
        indexPtr += This->IAIndexFormat;
        for (uint32_t i = 0; i < MSize; i++)
        {
			VINPUT_ELEMENT* x = &This->IAInputLayout->il[i];

            size_t index = (*OutStart).SV_VertexID * This->IABufferStrides[x->InputSlot] + This->IABufferOffsets[x->InputSlot] + x->AlignedByteOffset;
			uint8_t* data = &This->IAVertexBuffers[x->InputSlot]->data[index];

            switch (x->Format)
            {
            case FORMAT_R32G32B32A32_FLOAT:
				(*OutStart).data[i].v = VMLoadFloat4((VMFLOAT4*)(data));
                break;
            case FORMAT_R32G32B32_FLOAT:
				(*OutStart).data[i].v = VMLoadFloat3((VMFLOAT3*)(data));
                break;
            case FORMAT_R32G32_FLOAT:
				(*OutStart).data[i].v = VMLoadFloat2((VMFLOAT2*)(data));
                break;
            case FORMAT_R32_FLOAT:
				(*OutStart).data[i].v = VMLoadFloat((float*)(data));
                break;
            default:
                break;
            }
        }
        OutStart++;
    }
}

void InputADestroy(InputAssembler* This)
{
    ComPtrRelease(This->IAIndexBuffer);
    ComPtrRelease(This->IAInputLayout);
    for (uint32_t i = 0; i < MaxBuffers; i++)
    {
        ComPtrRelease(This->IAVertexBuffers[i]);
    }
    
}

