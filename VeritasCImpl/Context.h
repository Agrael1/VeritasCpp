#pragma once
#include "InputAssembly.h"
#include "Rasterizer.h"

#define c_class VContext

class
{
    COM_INTERFACE(IVContext);

    /* COMPtr */ IVShader* VSVertexShader;
    /* ComPtr */ VBuffer*  VSConstantBuffers[MaxBuffers];
    
    VRTV_DESC OMRenderTargets[MaxRenderTargets];
    VDSV_DESC OMRenderDepth;
    InputAssembler IAStage;
    Rasterizer RSStage;
};

#if !defined(CONTEXT_IMPL)
#undef c_class
#endif