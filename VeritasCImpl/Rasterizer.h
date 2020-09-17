#pragma once
#include "CommonTypes.h"
#include "Buffer.h"


typedef struct Rasterizer
{
    VMFLOAT4 RSVPScale;
    VMFLOAT4 RSVPOffset;
    VVIEWPORT_DESC RSViewPort;
    /* COMPtr */ IVShader* PSPixelShader;
    /* ComPtr */ VBuffer* PSConstantBuffers[MaxBuffers];
}Rasterizer;

HRESULT RasterSetViewPort(Rasterizer* This, uint32_t numVPs, const VVIEWPORT_DESC* in);
HRESULT __stdcall RasterSetShader(Rasterizer* This, IVShader* pPixelShader);
HRESULT __stdcall RasterSetConstantBuffers(Rasterizer* This, uint32_t StartSlot, uint32_t NumBuffers, IVBuffer* const* ppConstantBuffers);

size_t RasterGetSize(const Rasterizer* This);
void RasterizeTriangle(Rasterizer* This, VMVertex* TriangleStart, uint32_t PosCoord, uint32_t voSize);