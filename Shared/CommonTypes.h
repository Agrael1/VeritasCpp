#pragma once
#include <Veritas Math/VeritasMath.h>

typedef struct VMVertex
{
    XMVECTORF32 data[16];
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
inline VMVertex VSOutInterpolate(const VMVertex* v0, const VMVertex* v1, float alpha, size_t voSize)
{
    VMVertex out;

    for (size_t i = 0; i < voSize; i++)
    {
        out.data[i].v = VMVectorLerp(v0->data[i].v, v1->data[i].v, alpha);
    }
    out.SV_VertexID = 0;
    return out;
}
inline VMVertex VSOutSubtract(const VMVertex* v0, const VMVertex* v1, size_t voSize)
{
    VMVertex out;
    out.SV_VertexID = 0;
    while (voSize--)
    {
        out.data[voSize].v = VMVectorSubtract(v0->data[voSize].v, v1->data[voSize].v);
    }
    return out;
}
inline VMVertex VSOutMultiply(VMVertex v0, FVMVECTOR vec, size_t voSize)
{
    VMVertex out;
    out.SV_VertexID = 0;
    while (voSize--)
    {
        out.data[voSize].v = VMVectorMultiply(v0.data[voSize].v, vec);
    }
    return out;
}
inline VMVertex VSOutScale_I(VMVertex* v0, float x, size_t voSize)
{
    while (voSize--)
    {
        v0->data[voSize].v = VMVectorScale(v0->data[voSize].v, x);
    }
}
inline VMVertex VSOutAdd_I(VMVertex* v0, VMVertex v1, size_t voSize)
{
    while (voSize--)
    {
        v0->data[voSize].v = VMVectorAdd(v0->data[voSize].v, v1.data[voSize].v);
    }
}


inline void swapptr(void** x, void** y)
{
    void* z = *x;
    *x = *y;
    *y = z;
}

inline int xmaxi(int a, int b)
{
    return a > b ? a : b;
}
inline int xmini(int a, int b)
{
    return a < b ? a : b;
}

#define max(a,b) _Generic((a), \
int:    xmaxi\
)(a,b)
#define min(a,b) _Generic((a), \
int:    xmini\
)(a,b)