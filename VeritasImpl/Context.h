#pragma once
#include <array>
#include <span>
#include <algorithm>
#include <atomic>

#include "Buffer.h"
#include "InputLayout.h"
#include "Texture.h"

#include <VeritasMath.h>

namespace dx = DirectX;

struct XMVertex
{
    dx::XMVECTOR data[16];
    uint32_t SV_VertexID;
};
struct XMVSOut
{
public:
    float4& Position()
    {
        return attributes[SV_PosCoord];
    }
    const float4& Position()const
    {
        return attributes[SV_PosCoord];
    }
    void Scale(const float4& factor, uint32_t attrsize)
    {
        using namespace DirectX;
        while (attrsize--)
        {
            attributes[attrsize] = attributes[attrsize] * factor;
        }
    }
    void Increase(const XMVSOut& other, uint32_t size)
    {
        for (uint32_t i = 0; i < size; i++)
        {
            attributes[i] = attributes[i] + other[i];
        }
    }
public:
    float4& operator[](uint32_t sz)
    {
        return attributes[sz];
    }
    constexpr const float4& operator[](uint32_t sz)const
    {
        return attributes[sz];
    }
public:
    static XMVSOut Subtract(const XMVSOut& from, const XMVSOut& what, uint32_t attrsize)
    {
        XMVSOut out;
        out.SV_PosCoord = from.SV_PosCoord;
        while (attrsize--)
        {
            out[attrsize] = from[attrsize] - what[attrsize];
        }
        return out;
    }
    static XMVSOut Multiply(const XMVSOut& lhs, DirectX::FXMVECTOR rhs, uint32_t size)
    {
        using namespace DirectX;
        XMVSOut out;
        for (uint32_t i = 0; i < size; i++)
        {
            out[i] = lhs[i] * rhs;
        }
        return out;
    }
public:
    float4 attributes[16];
    uint32_t SV_PosCoord;
};

inline XMVSOut VSOutInterpolate(const XMVSOut& v0, const XMVSOut& v1, float alpha, size_t voSize)
{
    using namespace DirectX;
    XMVSOut out;

    for (size_t i = 0; i < voSize; i++)
    {
        out.attributes[i] = XMVectorLerp(v0.attributes[i], v1.attributes[i], alpha);
    }
    out.SV_PosCoord = v0.SV_PosCoord;

    return out;
}


class InputAssembler
{
public:
    InputAssembler() = default;
public:
    HRESULT SetIndexBuffer(IVBuffer* indexBuffer, VFORMAT format, uint32_t offsetBytes);
    HRESULT SetVertexBuffers(uint32_t StartSlot, uint32_t NumBuffers, IVBuffer* const* ppVertexBuffers, const uint32_t* pStrides, const uint32_t* pOffsets);
    HRESULT SetInputLayout(IVInputLayout* pInputLayout);
public:
    std::vector<XMVertex> MakeVertices(uint32_t NumVertices)const
    {
        assert(NumVertices % 3 == 0);
        std::vector<XMVertex> out;
        out.resize(NumVertices);

        for (uint32_t vtx = 0; auto & v : out)
        {
            for (uint32_t i = 0; i < GetMonotonicSize(); i++)
            {
                auto& x = IAInputLayout->il[i];
                size_t index = vtx * IABufferStrides[x.InputSlot] + IABufferOffsets[x.InputSlot] + x.AlignedByteOffset;
                auto* data = &IAVertexBuffers[x.InputSlot]->data[index];

                switch (x.Format)
                {
                case FORMAT_R32G32B32A32_FLOAT:
                    v.data[i] = dx::XMLoadFloat4(reinterpret_cast<dx::XMFLOAT4*>(data));
                    break;
                case FORMAT_R32G32B32_FLOAT:
                    v.data[i] = dx::XMLoadFloat3(reinterpret_cast<dx::XMFLOAT3*>(data));
                    break;
                case FORMAT_R32G32_FLOAT:
                    v.data[i] = dx::XMLoadFloat2(reinterpret_cast<dx::XMFLOAT2*>(data));
                    break;
                case FORMAT_R32_FLOAT:
                    v.data[i] = dx::XMLoadFloat(reinterpret_cast<float*>(data));
                    break;
                default:
                    break;
                }
            }
            v.SV_VertexID = vtx++;
        }
        return out;
    }
    std::vector<XMVertex> MakeVerticesIndexed(uint32_t NumIndices)const
    {
        assert(NumIndices % 3 == 0);
        assert(IAIndexBuffer);
        std::vector<XMVertex> out;
        out.resize(NumIndices);
        auto* indexPtr = &IAIndexBuffer->data[0];

        for (auto & v : out)
        {
            v.SV_VertexID = (*((uint32_t*)indexPtr)) & ((1ull << IAIndexFormat * CHAR_BIT) - 1);
            indexPtr += IAIndexFormat;
            for (uint32_t i = 0; i < GetMonotonicSize(); i++)
            {
                auto& x = IAInputLayout->il[i];

                size_t index = v.SV_VertexID * IABufferStrides[x.InputSlot] + IABufferOffsets[x.InputSlot] + x.AlignedByteOffset;
                auto* data = &IAVertexBuffers[x.InputSlot]->data[index];

                switch (x.Format)
                {
                case FORMAT_R32G32B32A32_FLOAT:
                    v.data[i] = dx::XMLoadFloat4(reinterpret_cast<dx::XMFLOAT4*>(data));
                    break;
                case FORMAT_R32G32B32_FLOAT:
                    v.data[i] = dx::XMLoadFloat3(reinterpret_cast<dx::XMFLOAT3*>(data));
                    break;
                case FORMAT_R32G32_FLOAT:
                    v.data[i] = dx::XMLoadFloat2(reinterpret_cast<dx::XMFLOAT2*>(data));
                    break;
                case FORMAT_R32_FLOAT:
                    v.data[i] = dx::XMLoadFloat(reinterpret_cast<float*>(data));
                    break;
                default:
                    break;
                }
            }
        }
        return out;
    }
    uint32_t GetMonotonicSize()const
    {
        return (uint32_t)IAInputLayout->monotonicSize;
    }
private:
    wrl::ComPtr<VInputLayout> IAInputLayout;
    wrl::ComPtr<VBuffer> IAIndexBuffer;
    VFORMAT IAIndexFormat = FORMAT_NONE;
    uint32_t IAIndexOffset = 0;
    std::array<wrl::ComPtr<VBuffer>, 4> IAVertexBuffers{ 0 };
    std::array<uint32_t, 4> IABufferStrides{ 0 };
    std::array<uint32_t, 4> IABufferOffsets{ 0 };


    //VPRIMITIVE_TOPOLOGY IATopology; //TODO: make topology
};

class VContext : public wrl::RuntimeClass<wrl::RuntimeClassFlags<wrl::ClassicCom>, IVContext>
{
public:
    VContext() = default;
public:
    HRESULT __stdcall IASetPrimitiveTopology(VPRIMITIVE_TOPOLOGY Topology)override;
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
    float4 RSVPScale;
    float4 RSVPOffset;

    InputAssembler IAStage;
    wrl::ComPtr<IVShader> VSVertexShader;
    wrl::ComPtr<IVShader> PSPixelShader;
    std::array<wrl::ComPtr<VBuffer>, 4> VSConstantBuffers;
    std::array<wrl::ComPtr<VBuffer>, 4> PSConstantBuffers;
    VVIEWPORT_DESC RSViewPort{ 0 };

    std::array<VRTV_DESC, MaxRenderTargets> OMRenderTargets;
    VDSV_DESC OMRenderDepth{ 0 };
};