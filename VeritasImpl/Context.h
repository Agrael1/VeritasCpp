#pragma once
#include <Interfaces.h>
#include <array>
#include <span>
#include <algorithm>
#include <atomic>

#include "Buffer.h"
#include "InputLayout.h"
#include "Texture.h"

#include <DirectXMath\Inc\DirectXPackedVector.h>

namespace dx = DirectX;

struct float4 : public dx::XMFLOAT4A
{
    float4() = default;
    float4(const dx::XMVECTOR in)
    {
        dx::XMStoreFloat4A(this, in);
    }
    float4 operator=(const dx::XMVECTOR in)
    {
        dx::XMStoreFloat4A(this, in);
        return *this;
    }
    operator dx::XMVECTOR()const
    {
        return dx::XMLoadFloat4A(this);
    }
};

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
        
        for (uint32_t vtx = 0; auto& v : out)
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
        return std::move(out);
    }
    std::vector<XMVertex> MakeVerticesIndexed(uint32_t NumIndices)const
    {
        assert(NumIndices % 3 == 0);
        assert(IAIndexBuffer);
        std::vector<XMVertex> out;
        out.resize(NumIndices);
        auto* indexPtr = &IAIndexBuffer->data[0];

        for (uint32_t idx = 0; auto & v : out)
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
    std::array<wrl::ComPtr<VBuffer>, 4> IAVertexBuffers{0};
    std::array<uint32_t, 4> IABufferStrides{0};
    std::array<uint32_t, 4> IABufferOffsets{0};


    //VPRIMITIVE_TOPOLOGY IATopology; //TODO: make topology
};

class VContext: public wrl::RuntimeClass<wrl::RuntimeClassFlags<wrl::ClassicCom>, IVContext>
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
    void __stdcall DrawIndexed(uint32_t nofVertices)override
    {
        auto verts = IAStage.MakeVerticesIndexed(nofVertices);

        std::array<void*, 4> x{};
        for (size_t i = 0; i<4; i++)
        {
            x[i] = VSConstantBuffers[i] ? VSConstantBuffers[i]->data.data() : nullptr;
        }
        VSVertexShader->UpdateConstants(x.data());

        for (size_t i = 0; i < 4; i++)
        {
            x[i] = PSConstantBuffers[i] ? PSConstantBuffers[i]->data.data() : nullptr;
        }
        PSPixelShader->UpdateConstants(x.data());

        std::vector<XMVSOut> VSOut;
        VSOut.resize(verts.size());

        for (uint32_t i = 0; auto & v : verts)
        {
            VSVertexShader->Invoke(&v, &VSOut[i++]);
        }
        AssembleTriangles(VSOut);
    }
private:
    void AssembleTriangles(std::vector<XMVSOut>& VSOut)
    {
        uint32_t vosize = 0;
        VSVertexShader->GetMonotonicSize(&vosize);
        for (size_t it = 0u; it < VSOut.size(); it += 3)
        {
            auto& v0 = VSOut[it];
            auto& v1 = VSOut[it + 1];
            auto& v2 = VSOut[it + 2];

            RSClipCullTriangles(v0, v1, v2, vosize);
        }
    }
    void RSClipCullTriangles(XMVSOut& v0, XMVSOut& v1, XMVSOut& v2, uint32_t vosize)
    {
        uint32_t idx = v0.SV_PosCoord;
        constexpr dx::XMVECTORI32 preor = {0x80000000, 0x80000000, 0x00000000, 0x00000000 };
        constexpr dx::XMVECTORI32 preand = { 0xFFFFFFFF, 0xFFFFFFFF, 0x00000000, 0xFFFFFFFF };

        using namespace DirectX;
        const auto Clip1V = [=](XMVSOut& v0, XMVSOut& v1, XMVSOut& v2)
        {
            const float alphaA = (-v0.attributes[idx].z) / (v1.attributes[idx].z - v0.attributes[idx].z);
            const float alphaB = (-v0.attributes[idx].z) / (v2.attributes[idx].z - v0.attributes[idx].z);
            // interpolate to get v0a and v0b
            auto v0a = VSOutInterpolate(v0, v1, alphaA, vosize);
            auto v0b = VSOutInterpolate(v0, v2, alphaB, vosize);
            auto v0a2 = v0a;
            auto v2b = v2;
            // draw triangles
            RSPostProcessTriangle(v0a, v1, v2, vosize);
            RSPostProcessTriangle(v0b, v0a2, v2b, vosize);
        };
        const auto Clip2V = [=](XMVSOut& v0, XMVSOut& v1, XMVSOut& v2)
        {
            // calculate alpha values for getting adjusted vertices
            const float alpha0 = (-v0.attributes[idx].z) / (v2.attributes[idx].z - v0.attributes[idx].z);
            const float alpha1 = (-v1.attributes[idx].z) / (v2.attributes[idx].z - v1.attributes[idx].z);
            // interpolate to get v0a and v0b

            v0 = VSOutInterpolate(v0, v2, alpha0, vosize);
            v1 = VSOutInterpolate(v1, v2, alpha1, vosize);
            // draw triangles
            RSPostProcessTriangle(v0, v1, v2, vosize);
        };

        XMVECTOR V0 = v0.attributes[idx];
        XMVECTOR V1 = v1.attributes[idx];
        XMVECTOR V2 = v2.attributes[idx];

        // Compare againgst W value
        XMVECTOR CT0 = XMVectorSplatW(V0);
        XMVECTOR CT1 = XMVectorSplatW(V1);
        XMVECTOR CT2 = XMVectorSplatW(V2);

        XMVECTOR R01 = XMVectorLess(CT0, V0);
        XMVECTOR R11 = XMVectorLess(CT1, V1);
        XMVECTOR R21 = XMVectorLess(CT2, V2);

        XMVECTOR RR1 = XMVectorAndInt(XMVectorAndInt(R01, R11), R21);
        if (_mm_movemask_ps(RR1) != 0) return;

        CT0 = _mm_and_ps(_mm_or_ps(CT0, preor), preand);
        CT1 = _mm_and_ps(_mm_or_ps(CT1, preor), preand);
        CT2 = _mm_and_ps(_mm_or_ps(CT2, preor), preand);

        XMVECTOR R02 = XMVectorLess(V0, CT0);
        XMVECTOR R12 = XMVectorLess(V1, CT1);
        XMVECTOR R22 = XMVectorLess(V2, CT2);
        XMVECTOR RR2 = XMVectorAndInt(XMVectorAndInt(R02, R12), R22);
        if (_mm_movemask_ps(RR2) != 0) return;

        RR1 = XMVectorMergeZW(R02, R12);
        RR2 = _mm_shuffle_ps(RR1, R22, _MM_SHUFFLE(3, 2, 1, 0));

        int selector = _mm_movemask_ps(RR2) & 7;
        switch (selector)
        {
        case 0: RSPostProcessTriangle(v0, v1, v2, vosize); break;
        case 1: Clip1V(v0, v1, v2); break;
        case 2: Clip1V(v1, v2, v0); break;
        case 3: Clip2V(v0, v1, v2); break;
        case 4: Clip1V(v2, v0, v1); break;
        case 5: Clip2V(v2, v0, v1); break;
        case 6: Clip2V(v1, v2, v0); break;
        }
    }
    void RSPostProcessTriangle(XMVSOut& v0, XMVSOut& v1, XMVSOut& v2, uint32_t attrsize)
    {
        using namespace DirectX;
        const uint32_t idx = v0.SV_PosCoord;
        // homo -> NDC space transformation
        XMVECTOR wInv = XMVectorReciprocal(_mm_shuffle_ps(XMVectorMergeZW(v0.attributes[idx], v1.attributes[idx]), v2.attributes[idx], _MM_SHUFFLE(3, 3, 3, 2)));
        XMMATRIX X;
        X.r[0] = XMVectorSplatX(wInv); // 1/w0
        X.r[1] = XMVectorSplatY(wInv); // 1/w1
        X.r[2] = XMVectorSplatZ(wInv); // 1/w2

        //Screen space transform, 1/w is stored in W
        v0[idx] = XMVectorMultiplyAdd(v0[idx]* X.r[0], RSVPScale, RSVPOffset); XMStoreFloat(&v0[idx].w, X.r[0]);
        v1[idx] = XMVectorMultiplyAdd(v1[idx]* X.r[1], RSVPScale, RSVPOffset); XMStoreFloat(&v1[idx].w, X.r[1]);
        v2[idx] = XMVectorMultiplyAdd(v2[idx]* X.r[2], RSVPScale, RSVPOffset); XMStoreFloat(&v2[idx].w, X.r[2]);

        //cull backfaces
        if (float4(XMVector3Cross((v1[idx] - v0[idx]), (v2[idx] - v0[idx]))).z < 0.0f)
            return;

        for (unsigned i = 0; i < attrsize; i++)
        {
            if (i == idx) continue;
            v0.attributes[i] = v0.attributes[i] * X.r[0];
            v1.attributes[i] = v1.attributes[i] * X.r[1];
            v2.attributes[i] = v2.attributes[i] * X.r[2];
        }
        DrawTriangle(v0, v1, v2, attrsize);
    }

    void DrawTriangle(const XMVSOut& Vo0, const XMVSOut& Vo1, const XMVSOut& Vo2, uint32_t attrsize)
    {
        const uint32_t idx = Vo0.SV_PosCoord;
        // using pointers so we can swap (for sorting purposes)
        const auto* pv0 = &Vo0;
        const auto* pv1 = &Vo1;
        const auto* pv2 = &Vo2;

        // sorting vertices by y
        if (pv1->attributes[idx].y < pv0->attributes[idx].y) std::swap(pv0, pv1);
        if (pv2->attributes[idx].y < pv1->attributes[idx].y) std::swap(pv1, pv2);
        if (pv1->attributes[idx].y < pv0->attributes[idx].y) std::swap(pv0, pv1);

        if (pv0->attributes[idx].y == pv1->attributes[idx].y) // natural flat top
        {
            // sorting top vertices by x
            if (pv1->attributes[idx].x < pv0->attributes[idx].x) std::swap(pv0, pv1);

            DrawFlatTopTriangle(*pv0, *pv1, *pv2, attrsize);
        }
        else if (pv1->attributes[idx].y == pv2->attributes[idx].y) // natural flat bottom
        {
            // sorting bottom vertices by x
            if (pv2->attributes[idx].x < pv1->attributes[idx].x) std::swap(pv1, pv2);

            DrawFlatBottomTriangle(*pv0, *pv1, *pv2, attrsize);
        }
        else // general triangle
        {
            // find splitting vertex interpolant
            const float alphaSplit =
                (pv1->attributes[idx].y - pv0->attributes[idx].y) /
                (pv2->attributes[idx].y - pv0->attributes[idx].y);
            const auto vi = VSOutInterpolate(*pv0, *pv2, alphaSplit, attrsize);

            if (pv1->attributes[idx].x < vi.attributes[idx].x) // major right
            {
                DrawFlatBottomTriangle(*pv0, *pv1, vi, attrsize);
                DrawFlatTopTriangle(*pv1, vi, *pv2, attrsize);
            }
            else // major left
            {
                DrawFlatBottomTriangle(*pv0, vi, *pv1, attrsize);
                DrawFlatTopTriangle(vi, *pv1, *pv2, attrsize);
            }
        }
    }
    void DrawFlatTopTriangle(const XMVSOut& Vo0, const XMVSOut& Vo1, const XMVSOut& Vo2, uint32_t attrsize)
    {
        using namespace DirectX;
        XMVSOut InterpolantEdge = Vo1;
        // calulcate dVertex / dy
        // change in interpolant for every 1 change in y
        auto dv0 = XMVSOut::Subtract(Vo2, Vo0, attrsize);
        auto dv1 = XMVSOut::Subtract(Vo2, Vo1, attrsize);
        FXMVECTOR delta_Y = XMVectorSplatY(XMVectorReciprocal(dv0.Position()));

        // delta over 0 and 1 resp
        dv0.Scale(delta_Y, attrsize);
        dv1.Scale(delta_Y, attrsize);

        // call the flat triangle render routine, right edge interpolant is it1
        DrawFlatTriangle(Vo0, Vo2, dv0, dv1, InterpolantEdge, attrsize);
    }
    void DrawFlatBottomTriangle(const XMVSOut& Vo0, const XMVSOut& Vo1, const XMVSOut& Vo2, uint32_t attrsize)
    {
        using namespace DirectX;
        XMVSOut InterpolantEdge = Vo0;

        // calulcate dVertex / dy
        // change in interpolant for every 1 change in y
        auto dv0 = XMVSOut::Subtract(Vo1, Vo0, attrsize);
        auto dv1 = XMVSOut::Subtract(Vo2, Vo0, attrsize);
        FXMVECTOR delta_Y = XMVectorReciprocal(XMVectorSplatY(dv0.Position())); // minimize div (reciprocalEst is not good enough)

        // delta over 1 and 2 resp
        dv0.Scale(delta_Y, attrsize);
        dv1.Scale(delta_Y, attrsize);

        // call the flat triangle render routine, right edge interpolant is it0
        DrawFlatTriangle(Vo0, Vo2, dv0, dv1, InterpolantEdge, attrsize);
    }

    void DrawFlatTriangle(const XMVSOut& it0,
        const XMVSOut& it2,
        const XMVSOut& dv0,
        const XMVSOut& dv1,
        XMVSOut& itEdge1, uint32_t attrsize)
    {
        using namespace DirectX;

        // create edge interpolant for left edge (always v0)
        XMVSOut itEdge0 = it0;
        XMVSOut iLine;
        XMVSOut diLine;
        XMVSOut _P;

        // calculate start and end scanlines (AABB)
        const int yStart = std::max((int)ceil(it0.Position().y - 0.5f), 0);
        const int yEnd = std::min((int)ceil(it2.Position().y - 0.5f), (int)RSViewPort.Height); // the scanline AFTER the last line drawn

        // do interpolant prestep
        FXMVECTOR step = XMVectorReplicate(((float)yStart + 0.5f - it0.Position().y));

        for (unsigned i = 0; i < attrsize; i++)
        {
            itEdge0[i] = XMVectorMultiplyAdd(dv0[i], step, itEdge0[i]);
            itEdge1[i] = XMVectorMultiplyAdd(dv1[i], step, itEdge1[i]);
        }

        for (int y = yStart; y < yEnd; y++, itEdge0.Increase(dv0, attrsize), itEdge1.Increase(dv1, attrsize))
        {
            // calculate start and end pixels
            const int xStart = std::max((int)ceil(itEdge0.Position().x - 0.5f), 0);
            const int xEnd = std::min((int)ceil(itEdge1.Position().x - 0.5f), (int)RSViewPort.Width); // the pixel AFTER the last pixel drawn

            // create scanline interpolant startpoint
            // (some waste for interpolating x,y,z, but makes life easier not having
            //  to split them off, and z will be needed in the future anyways...)

            iLine = itEdge0;
            FXMVECTOR step2 = XMVectorReplicate((float)xStart + 0.5f - itEdge0.Position().x);
            FXMVECTOR Delta_X = XMVectorReciprocal(XMVectorSplatX(itEdge1.Position() - itEdge0.Position()));
            diLine = XMVSOut::Multiply(XMVSOut::Subtract(itEdge1, iLine, attrsize), Delta_X, attrsize);
            iLine.Increase(XMVSOut::Multiply(diLine, step2, attrsize), attrsize);

            const size_t premulI = y * size_t(RSViewPort.Width);


            for (int x = xStart; x < xEnd; x++, iLine.Increase(diLine, attrsize))
            {
                if (auto [pass, zv] = DepthTest(x, premulI, iLine.Position().z); pass)
                {
                    // recover interpolated z from interpolated 1/z
                    FXMVECTOR w = XMVectorReciprocalEst(XMVectorSplatW(iLine.Position()));
                    for (unsigned i = 0; i < attrsize; i++)
                        _P[i] = iLine[i] * w;
                    // invoke pixel shader with interpolated vertex attributes
                    // and use result to set the pixel color on the screen
                    PackedVector::XMCOLOR col;
                    PSPixelShader->Invoke(&_P, &col);

                    if (zv == ((float*)OMRenderDepth.Scan0)[premulI + x]) //afxmt sanity check for data races
                    {
                        ((uint32_t*)OMRenderTargets[0].Scan0)[premulI + x] = col;
                    }

                }
            }
        }
    }
    std::pair<bool, float> DepthTest(uint32_t width_in, size_t PremulIndex, float z)
    {
        //Warning! lockless programming!
        auto* reg = reinterpret_cast<std::atomic<float>*>(&((float*)OMRenderDepth.Scan0)[PremulIndex + width_in]);
        auto zv = std::atomic_load(reg);

        do
        {
            if (z >= zv) return { false, z };
        } while (!std::atomic_compare_exchange_weak(reg, &zv, z));

        return { true, z };
    }

private:
    InputAssembler IAStage;
    wrl::ComPtr<IVShader> VSVertexShader;
    wrl::ComPtr<IVShader> PSPixelShader;
    std::array<wrl::ComPtr<VBuffer>, 4> VSConstantBuffers;
	std::array<wrl::ComPtr<VBuffer>, 4> PSConstantBuffers;
    VVIEWPORT_DESC RSViewPort{ 0 };
    dx::XMVECTOR RSVPScale{ 0 };
    dx::XMVECTOR RSVPOffset{ 0 };

    std::array<VRTV_DESC, MaxRenderTargets> OMRenderTargets{ 0 };
    VDSV_DESC OMRenderDepth{ 0 };
};