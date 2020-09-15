#pragma once
#include "Allocator.h"
#include "InputLayout.h"
#include "Buffer.h"
#include "CommonTypes.h"

typedef struct InputAssembler
{
    VInputLayout* IAInputLayout;
    VBuffer* IAIndexBuffer;
    VBuffer* IAVertexBuffers[MaxBuffers];

    uint32_t IAIndexOffset;
    VFORMAT IAIndexFormat;
    uint32_t IABufferStrides[MaxBuffers];
    uint32_t IABufferOffsets[MaxBuffers];
}InputAssembler;

inline void ComPtrRelease(IUnknown* This)
{
    if (This) This->method->Release(This);
}

HRESULT __stdcall InputASetPrimitiveTopology(InputAssembler* This, VPRIMITIVE_TOPOLOGY Topology);
HRESULT __stdcall InputASetVertexBuffers(InputAssembler* This, uint32_t StartSlot, uint32_t NumBuffers, IVBuffer* const* ppVertexBuffers, const uint32_t* pStrides, const uint32_t* pOffsets);
HRESULT __stdcall InputASetIndexBuffer(InputAssembler* This, IVBuffer* indexBuffer, VFORMAT format, uint32_t offsetBytes);
HRESULT __stdcall InputASetInputLayout(InputAssembler* This, IVInputLayout* pInputLayout);

uint32_t InputAGetMonotonicSize(const InputAssembler* This);
void InputAMakeVertices(const InputAssembler* This, uint32_t NumVertices, VMVertex* _out_VArray);
void InputAMakeVerticesIndexed(const InputAssembler* This, uint32_t NumIndices, VMVertex* _out_VArray);

void InputADestroy(InputAssembler* This);
