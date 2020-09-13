#pragma once
#include "InputAssembly.h"
#include "Veritas Math/VeritasMath.h"

#define c_class VContext

class
{
    COM_INTERFACE(IVContext);
    /*HRESULT __stdcall IASetPrimitiveTopology(VPRIMITIVE_TOPOLOGY Topology)override;
    HRESULT __stdcall IASetVertexBuffers(uint32_t StartSlot, uint32_t NumBuffers, IVBuffer* const* ppVertexBuffers, const uint32_t* pStrides, const uint32_t* pOffsets)override;
    HRESULT __stdcall IASetIndexBuffer(IVBuffer* indexBuffer, VFORMAT format, uint32_t offsetBytes)override;
    HRESULT __stdcall IASetInputLayout(IVInputLayout* pInputLayout)override;
    HRESULT __stdcall VSSetShader(IVShader* pVertexShader)override;
    HRESULT __stdcall VSSetConstantBuffers(uint32_t StartSlot, uint32_t NumBuffers, IVBuffer* const* ppConstantBuffers)override;
    HRESULT __stdcall PSSetShader(IVShader* pPixelShader)override;
    HRESULT __stdcall PSSetConstantBuffers(uint32_t StartSlot, uint32_t NumBuffers, IVBuffer* const* ppConstantBuffers)override;
    HRESULT __stdcall RSSetViewport(uint32_t numVPs, const VVIEWPORT_DESC* in)override;
    HRESULT __stdcall OMSetRenderTargets(uint32_t numViews, const VRTV_DESC* const _arr_RTVs)override;
    HRESULT __stdcall OMSetDepthStencil(const VDSV_DESC* DSV)override;

    HRESULT __stdcall ClearRenderTarget(VRTV_DESC* rtv, uint32_t col)override;
    HRESULT __stdcall ClearDepthStencil(VDSV_DESC* dsv, float value)override;
    HRESULT __stdcall Map(IVBuffer* pResource, VMAPPED_SUBRESOURCE* _out_pMappedResource)override;
    HRESULT __stdcall Unmap(IVBuffer* pResource)override;

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



    wrl::ComPtr<IVShader> VSVertexShader;
    wrl::ComPtr<IVShader> PSPixelShader;
    std::array<wrl::ComPtr<VBuffer>, 4> VSConstantBuffers;
    std::array<wrl::ComPtr<VBuffer>, 4> PSConstantBuffers;
    { 0 };

    
    VDSV_DESC OMRenderDepth{ 0 };*/
    VMFLOAT4 RSVPScale;
    VMFLOAT4 RSVPOffset;
    VVIEWPORT_DESC RSViewPort;
    VRTV_DESC OMRenderTargets[MaxRenderTargets];
    InputAssembler IAStage;
};

#if !defined(CONTEXT_IMPL)
#undef c_class
#endif