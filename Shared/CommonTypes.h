#pragma once
#include <Veritas Math/VeritasMath.h>

typedef struct VMVertex
{
    VMVECTOR data[16];
    uint32_t SV_VertexID;
}VMVertex;


//struct XMVSOut
//{
//public:
//    float4& Position()
//    {
//        return attributes[SV_PosCoord];
//    }
//    const float4& Position()const
//    {
//        return attributes[SV_PosCoord];
//    }
//    void Scale(const float4& factor, uint32_t attrsize)
//    {
//        using namespace DirectX;
//        while (attrsize--)
//        {
//            attributes[attrsize] = attributes[attrsize] * factor;
//        }
//    }
//    void Increase(const XMVSOut& other, uint32_t size)
//    {
//        for (uint32_t i = 0; i < size; i++)
//        {
//            attributes[i] = attributes[i] + other[i];
//        }
//    }
//public:
//    float4& operator[](uint32_t sz)
//    {
//        return attributes[sz];
//    }
//    constexpr const float4& operator[](uint32_t sz)const
//    {
//        return attributes[sz];
//    }
//public:
//    static XMVSOut Subtract(const XMVSOut& from, const XMVSOut& what, uint32_t attrsize)
//    {
//        XMVSOut out;
//        out.SV_PosCoord = from.SV_PosCoord;
//        while (attrsize--)
//        {
//            out[attrsize] = from[attrsize] - what[attrsize];
//        }
//        return out;
//    }
//    static XMVSOut Multiply(const XMVSOut& lhs, DirectX::FXMVECTOR rhs, uint32_t size)
//    {
//        using namespace DirectX;
//        XMVSOut out;
//        for (uint32_t i = 0; i < size; i++)
//        {
//            out[i] = lhs[i] * rhs;
//        }
//        return out;
//    }
//public:
//    float4 attributes[16];
//    uint32_t SV_PosCoord;
//};
//
//inline XMVSOut VSOutInterpolate(const XMVSOut& v0, const XMVSOut& v1, float alpha, size_t voSize)
//{
//    using namespace DirectX;
//    XMVSOut out;
//
//    for (size_t i = 0; i < voSize; i++)
//    {
//        out.attributes[i] = XMVectorLerp(v0.attributes[i], v1.attributes[i], alpha);
//    }
//    out.SV_PosCoord = v0.SV_PosCoord;
//
//    return out;
//}