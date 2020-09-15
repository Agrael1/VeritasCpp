#pragma once
#include "InputAssembly.h"

#define c_class VContext

class
{
    COM_INTERFACE(IVContext);
    /*

    void __stdcall Draw(uint32_t nofVertices)override
    {

    }
    void __stdcall DrawIndexed(uint32_t nofVertices)override;
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
    /* COMPtr */ IVShader* PSPixelShader;
    /* ComPtr */ VBuffer*  VSConstantBuffers[MaxBuffers];
    /* ComPtr */ VBuffer*  PSConstantBuffers[MaxBuffers];
    VMFLOAT4 RSVPScale;
    VMFLOAT4 RSVPOffset;
    VVIEWPORT_DESC RSViewPort;
    VRTV_DESC OMRenderTargets[MaxRenderTargets];
    VDSV_DESC OMRenderDepth;
    InputAssembler IAStage;
};

#if !defined(CONTEXT_IMPL)
#undef c_class
#endif