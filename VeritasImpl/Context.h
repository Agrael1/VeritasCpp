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
    float4 attributes[16];
    uint32_t SV_PosCoord;
};
struct PSOut
{
    uint32_t SV_Target;
};

inline XMVSOut VSOutInterpolate(const XMVSOut& v0, const XMVSOut& v1, float alpha, size_t voSize)
{
    using namespace DirectX;
    XMVSOut out;

    for (size_t i = 0; i < voSize; i++)
    {
        out.attributes[i] = XMVectorLerp(v0.attributes[i], v1.attributes[i], alpha);
    }

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
        out.reserve(NumVertices);
        
        for (size_t vtx = 0; auto& v : out)
        {
            for (size_t i = 0; const auto& x : IAInputLayout->il)
            {
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
                i++;
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
        out.reserve(NumIndices);
        auto* indexPtr = &IAIndexBuffer->data[0];

        for (size_t idx = 0; auto& v : out)
        {
            for (size_t i = 0; const auto& x : IAInputLayout->il)
            {
                v.SV_VertexID = (*((uint32_t*)indexPtr) & ((1u << IAIndexFormat * CHAR_BIT) - 1));
                indexPtr += IAIndexFormat;
                size_t index = v.SV_VertexID * IABufferStrides[x.InputSlot] + IABufferOffsets[x.InputSlot] + x.AlignedByteOffset;
                auto* data = &IAVertexBuffers[x.InputSlot]->data[index];

                switch (x.Format)
                {
                case FORMAT_R32G32B32A32_FLOAT:
                    v.data[i] = dx::XMLoadFloat4(reinterpret_cast<dx::XMFLOAT4*>(&IAVertexBuffers[x.InputSlot]->data[x.AlignedByteOffset]));
                    break;
                case FORMAT_R32G32B32_FLOAT:
                    v.data[i] = dx::XMLoadFloat3(reinterpret_cast<dx::XMFLOAT3*>(&IAVertexBuffers[x.InputSlot]->data[x.AlignedByteOffset]));
                    break;
                case FORMAT_R32G32_FLOAT:
                    v.data[i] = dx::XMLoadFloat2(reinterpret_cast<dx::XMFLOAT2*>(&IAVertexBuffers[x.InputSlot]->data[x.AlignedByteOffset]));
                    break;
                case FORMAT_R32_FLOAT:
                    v.data[i] = dx::XMLoadFloat(reinterpret_cast<float*>(&IAVertexBuffers[x.InputSlot]->data[x.AlignedByteOffset]));
                    break;
                default:
                    break;
                }
                i++;
            }
        }
    }
    uint32_t GetMonotonicSize()const
    {
        return IAInputLayout->il.size();
    }
private:
    wrl::ComPtr<VInputLayout> IAInputLayout;
    wrl::ComPtr<VBuffer> IAIndexBuffer;
    VFORMAT IAIndexFormat;
    uint32_t IAIndexOffset;
    std::array<wrl::ComPtr<VBuffer>, 4> IAVertexBuffers;
    std::array<uint32_t, 4> IABufferStrides;
    std::array<uint32_t, 4> IABufferOffsets;


    VPRIMITIVE_TOPOLOGY IATopology; //TODO: make topology
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
	HRESULT __stdcall RSSetViewports(uint32_t numVPs, const VVIEWPORT_DESC* _arr_VPs)override;
	HRESULT __stdcall OMSetRenderTargets(uint32_t numViews, const VRTV_DESC* const _arr_RTVs)override;
    HRESULT __stdcall OMSetDepthStencil(const VDSV_DESC* DSV)override;
	HRESULT __stdcall ClearRenderTarget(VRTV_DESC* rtv, uint32_t col)override;

    void __stdcall Draw(uint32_t nofVertices)override
    {
    }
    void __stdcall DrawIndexed(uint32_t nofVertices)override
    {
        auto verts = IAStage.MakeVerticesIndexed(nofVertices);
        std::vector<XMVSOut> VSOut;
        VSOut.reserve(verts.size());

        for (uint32_t i = 0; auto& v : verts)
        {
            VSVertexShader->Invoke(&v, &VSOut[i]);
        }
        AssembleTriangles(VSOut);
    }
private:
    void AssembleTriangles(std::vector<XMVSOut>& VSOut)
    {
        for (size_t it = 0u; it < VSOut.size(); it += 3)
        {
            auto& v0 = VSOut[it];
            auto& v1 = VSOut[it + 1];
            auto& v2 = VSOut[it + 2];

            ClipCullTriangles(v0, v1, v2);
        }
    }
    void ClipCullTriangles(XMVSOut& v0, XMVSOut& v1, XMVSOut& v2)
    {
        uint32_t vosize = IAStage.GetMonotonicSize();
        uint32_t idx = v0.SV_PosCoord;

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
            PostProcessTriangle(v0a, v1, v2);
            PostProcessTriangle(v0b, v0a2, v2b);
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
            PostProcessTriangle(v0, v1, v2);
        };

        float4 V0 = v0.attributes[idx];
        float4 V1 = v1.attributes[idx];
        float4 V2 = v2.attributes[idx];

        // Compare againgst W value
        XMVECTOR CT0 = XMVectorSplatW(V0);
        XMVECTOR CT1 = XMVectorSplatW(V1);
        XMVECTOR CT2 = XMVectorSplatW(V2);

        XMVECTOR R01 = XMVectorLess(CT0, V0);
        XMVECTOR R11 = XMVectorLess(CT1, V1);
        XMVECTOR R21 = XMVectorLess(CT2, V2);

        XMVECTOR RR1 = XMVectorAndInt(XMVectorAndInt(R01, R11), R21);
        if (_mm_movemask_ps(RR1) != 0) return;

        CT0 = XMVectorNegate(CT0);
        CT1 = XMVectorNegate(CT1);
        CT2 = XMVectorNegate(CT2);

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
        case 0: PostProcessTriangle(v0, v1, v2); break;
        case 1: Clip1V(v0, v1, v2); break;
        case 2: Clip1V(v1, v2, v0); break;
        case 3: Clip2V(v0, v1, v2); break;
        case 4: Clip1V(v2, v0, v1); break;
        case 5: Clip2V(v2, v0, v1); break;
        case 6: Clip2V(v1, v2, v0); break;
        }
    }
    void PostProcessTriangle(XMVSOut& v0, XMVSOut& v1, XMVSOut& v2)
    {
        using namespace DirectX;
        uint32_t idx = v0.SV_PosCoord;
        // homo -> NDC space transformation
        XMVECTOR wInv = XMVectorReciprocal(_mm_shuffle_ps(XMVectorMergeZW(v0.attributes[idx], v1.attributes[idx]), v2.attributes[idx], _MM_SHUFFLE(3, 3, 3, 2)));
        XMMATRIX X;
        X.r[0] = XMVectorSplatX(wInv); // 1/w0
        X.r[1] = XMVectorSplatY(wInv); // 1/w1
        X.r[2] = XMVectorSplatZ(wInv); // 1/w2

        //Screen space transform, 1/w is stored in W
        v0.attributes[idx] = XMVectorMultiplyAdd(XMVectorMultiply(v0.attributes[idx], X.r[0]), Scale, Offset); XMStoreFloat(&v0.attributes[idx].w, X.r[0]);
        v1.attributes[idx] = XMVectorMultiplyAdd(XMVectorMultiply(v1.attributes[idx], X.r[1]), Scale, Offset); XMStoreFloat(&v1.attributes[idx].w, X.r[1]);
        v2.attributes[idx] = XMVectorMultiplyAdd(XMVectorMultiply(v2.attributes[idx], X.r[2]), Scale, Offset); XMStoreFloat(&v2.attributes[idx].w, X.r[2]);

        //cull backfaces
        if (VMVector3Cross((v1.dx.SV_Position.v - v0.dx.SV_Position.v), (v2.dx.SV_Position.v - v0.dx.SV_Position.v)).f[2] < 0.0f)
            return;

        auto vosize = activeSP->GetMonotonicSize();
        for (unsigned i = 0; i < vosize; i++)
        {
            v0.dx.attributes[i].v = v0.dx.attributes[i].v * X.r[0];
            v1.dx.attributes[i].v = v1.dx.attributes[i].v * X.r[1];
            v2.dx.attributes[i].v = v2.dx.attributes[i].v * X.r[2];
        }
        DrawTriangle(v0, v1, v2);
    }

    void DrawTriangle(const XMVSOut& Vo0, const XMVSOut& Vo1, const XMVSOut& Vo2)
    {
        // using pointers so we can swap (for sorting purposes)
        const auto* pv0 = &Vo0;
        const auto* pv1 = &Vo1;
        const auto* pv2 = &Vo2;

        // sorting vertices by y
        if (pv1->dx.SV_Position.f[1] < pv0->dx.SV_Position.f[1]) std::swap(pv0, pv1);
        if (pv2->dx.SV_Position.f[1] < pv1->dx.SV_Position.f[1]) std::swap(pv1, pv2);
        if (pv1->dx.SV_Position.f[1] < pv0->dx.SV_Position.f[1]) std::swap(pv0, pv1);

        if (pv0->dx.SV_Position.f[1] == pv1->dx.SV_Position.f[1]) // natural flat top
        {
            // sorting top vertices by x
            if (pv1->dx.SV_Position.f[0] < pv0->dx.SV_Position.f[0]) std::swap(pv0, pv1);

            DrawFlatTopTriangle(*pv0, *pv1, *pv2);
        }
        else if (pv1->dx.SV_Position.f[1] == pv2->dx.SV_Position.f[1]) // natural flat bottom
        {
            // sorting bottom vertices by x
            if (pv2->dx.SV_Position.f[0] < pv1->dx.SV_Position.f[0]) std::swap(pv1, pv2);

            DrawFlatBottomTriangle(*pv0, *pv1, *pv2);
        }
        else // general triangle
        {
            // find splitting vertex interpolant
            const float alphaSplit =
                (pv1->dx.SV_Position.f[1] - pv0->dx.SV_Position.f[1]) /
                (pv2->dx.SV_Position.f[1] - pv0->dx.SV_Position.f[1]);
            const auto vi = VSOutInterpolate(*pv0, *pv2, alphaSplit, activeSP->GetMonotonicSize());

            if (pv1->dx.SV_Position.f[0] < vi.dx.SV_Position.f[0]) // major right
            {
                DrawFlatBottomTriangle(*pv0, *pv1, vi);
                DrawFlatTopTriangle(*pv1, vi, *pv2);
            }
            else // major left
            {
                DrawFlatBottomTriangle(*pv0, vi, *pv1);
                DrawFlatTopTriangle(vi, *pv1, *pv2);
            }
        }
    }
    void DrawFlatTopTriangle(const XMVSOut& Vo0, const XMVSOut& Vo1, const XMVSOut& Vo2)
    {
        using namespace DirectX;
        auto vosize = activeSP->GetMonotonicSize();
        XMVSOut InterpolantEdge = Vo1;
        // calulcate dVertex / dy
        // change in interpolant for every 1 change in y
        auto dv0 = XMVSOut::Subtract(Vo2, Vo0, vosize);
        auto dv1 = XMVSOut::Subtract(Vo2, Vo1, vosize);
        FXMVECTOR delta_Y = XMVectorSplatY(XMVectorReciprocal(dv0.dx.SV_Position));

        // delta over 0 and 1 resp
        dv0.Scale(delta_Y, vosize);
        dv1.Scale(delta_Y, vosize);

        // call the flat triangle render routine, right edge interpolant is it1
        DrawFlatTriangle(Vo0, Vo2, dv0, dv1, InterpolantEdge);
    }
    void DrawFlatBottomTriangle(const XMVSOut& Vo0, const XMVSOut& Vo1, const XMVSOut& Vo2)
    {
        using namespace DirectX;
        auto vosize = activeSP->GetMonotonicSize();
        XMVSOut InterpolantEdge = Vo0;

        // calulcate dVertex / dy
        // change in interpolant for every 1 change in y
        auto dv0 = XMVSOut::Subtract(Vo1, Vo0, vosize);
        auto dv1 = XMVSOut::Subtract(Vo2, Vo0, vosize);
        FXMVECTOR delta_Y = XMVectorReciprocal(XMVectorSplatY(dv0.dx.SV_Position)); // minimize div (reciprocalEst is not good enough)

        // delta over 1 and 2 resp
        dv0.Scale(delta_Y, vosize);
        dv1.Scale(delta_Y, vosize);

        // call the flat triangle render routine, right edge interpolant is it0
        DrawFlatTriangle(Vo0, Vo2, dv0, dv1, InterpolantEdge);
    }

    void DrawFlatTriangle(const XMVSOut& it0,
        const XMVSOut& it2,
        const XMVSOut& dv0,
        const XMVSOut& dv1,
        XMVSOut& itEdge1)
    {
        using namespace DirectX;
        const size_t Size = activeSP->GetMonotonicSize();

        // create edge interpolant for left edge (always v0)
        XMVSOut itEdge0 = it0;
        XMVSOut iLine;
        XMVSOut diLine;
        XMInPixel _P;

        // calculate start and end scanlines (AABB)
        const int yStart = std::max((int)ceil(it0.dx.SV_Position.f[1] - 0.5f), 0);
        const int yEnd = std::min((int)ceil(it2.dx.SV_Position.f[1] - 0.5f), (int)height); // the scanline AFTER the last line drawn

        // do interpolant prestep
        FXMVECTOR step = XMVectorReplicate(((float)yStart + 0.5f - it0.dx.SV_Position.f[1]));

        for (unsigned i = 0; i < Size; i++)
        {
            itEdge0.dx.attributes[i].v = XMVectorMultiplyAdd(dv0.dx.attributes[i], step, itEdge0.dx.attributes[i]);
            itEdge1.dx.attributes[i].v = XMVectorMultiplyAdd(dv1.dx.attributes[i], step, itEdge1.dx.attributes[i]);
        }
        itEdge0.dx.SV_Position.v = XMVectorMultiplyAdd(dv0.dx.SV_Position, step, itEdge0.dx.SV_Position);
        itEdge1.dx.SV_Position.v = XMVectorMultiplyAdd(dv1.dx.SV_Position, step, itEdge1.dx.SV_Position);

        for (int y = yStart; y < yEnd; y++, itEdge0.Increase(dv0, Size), itEdge1.Increase(dv1, Size))
        {
            // calculate start and end pixels
            const int xStart = std::max((int)ceil(itEdge0.dx.SV_Position.f[0] - 0.5f), 0);
            const int xEnd = std::min((int)ceil(itEdge1.dx.SV_Position.f[0] - 0.5f), (int)width); // the pixel AFTER the last pixel drawn

            // create scanline interpolant startpoint
            // (some waste for interpolating x,y,z, but makes life easier not having
            //  to split them off, and z will be needed in the future anyways...)

            iLine = itEdge0;
            FXMVECTOR step2 = XMVectorReplicate((float)xStart + 0.5f - itEdge0.dx.SV_Position.f[0]);
            FXMVECTOR Delta_X = XMVectorReciprocal(XMVectorSplatX(itEdge1.dx.SV_Position.v - itEdge0.dx.SV_Position.v));
            diLine = XMVSOut::Multiply(XMVSOut::Subtract(itEdge1, iLine, Size), Delta_X, Size);
            iLine += XMVSOut::Multiply(diLine, step2, Size);

            const size_t premulI = y * size_t(width);


            for (int x = xStart; x < xEnd; x++, iLine.Increase(diLine, Size))
            {
                if (auto [pass, zv] = DepthTest(x, premulI, iLine.dx.SV_Position.f[2]); pass)
                {
                    // recover interpolated z from interpolated 1/z
                    FXMVECTOR w = XMVectorReciprocalEst(XMVectorSplatW(iLine.dx.SV_Position));
                    for (unsigned i = 0; i < Size; i++)
                        _P.dx.dx.attributes[i].v = iLine.dx.attributes[i].v * w;
                    _P.dx.dx.SV_Position.v = iLine.dx.SV_Position;
                    // invoke pixel shader with interpolated vertex attributes
                    // and use result to set the pixel color on the screen
                    DirectX::PackedVector::XMCOLOR col;
                    XMStoreColor2(&col, activeSP->InvokePS(_P.gl).SV_Target);
                    if (zv == DepthStencil[premulI + x]) //afxmt sanity check for data races
                    {
                        FrameBuffer[premulI + x] = col;
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

	std::array<VRTV_DESC, MaxRenderTargets> OMRenderTargets;
    VDSV_DESC OMRenderDepth;
	std::array<VVIEWPORT_DESC, MaxViewPorts> RSViewPorts;
	wrl::ComPtr<IVShader> VSVertexShader;
	wrl::ComPtr<IVShader> PSPixelShader;
	std::vector<wrl::ComPtr<VBuffer>> VSConstantBuffers;
	std::vector<wrl::ComPtr<VBuffer>> PSConstantBuffers;
};