#pragma once
#include "InputAssembly.h"
#include "Rasterizer.h"

#define c_class VContext

class
{
    COM_INTERFACE(IVContext);
    /*
private:
    void AssembleTriangles(std::vector<XMVSOut>& VSOut);
    void RSClipCullTriangles(XMVSOut& v0, XMVSOut& v1, XMVSOut& v2, uint32_t vosize);
    void RSPostProcessTriangle(XMVSOut& v0, XMVSOut& v1, XMVSOut& v2, uint32_t attrsize);

    void DrawTriangle(const XMVSOut& Vo0, const XMVSOut& Vo1, const XMVSOut& Vo2, uint32_t attrsize);
    void DrawFlatTopTriangle(const XMVSOut& Vo0, const XMVSOut& Vo1, const XMVSOut& Vo2, uint32_t attrsize);
    void DrawFlatBottomTriangle(const XMVSOut& Vo0, const XMVSOut& Vo1, const XMVSOut& Vo2, uint32_t attrsize);

    void DrawFlatTriangle(const XMVSOut& it0,
        const XMVSOut& it2,
        const XMVSOut& dv0,
        const XMVSOut& dv1,
        XMVSOut& itEdge1, uint32_t attrsize);
    std::pair<bool, float> DepthTest(uint32_t width_in, size_t PremulIndex, float z);
private:

    */

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