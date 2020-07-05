#pragma once
#include "Descriptors.h"
#include <algorithm>
#include <vector>
#include <wrl.h>

namespace wrl = Microsoft::WRL;

// {979E8BE5-01F7-4264-99D1-7517ACD6B138}
DEFINE_GUID(IID_VBuffer,
    0x979e8be5, 0x1f7, 0x4264, 0x99, 0xd1, 0x75, 0x17, 0xac, 0xd6, 0xb1, 0x38);

MIDL_INTERFACE("979E8BE5-01F7-4264-99D1-7517ACD6B138")
IVBuffer : public IUnknown
{
    virtual void __stdcall GetDesc(VBUFFER_DESC* _out_Desc) = 0;
};


class VBuffer : public wrl::RuntimeClass<wrl::RuntimeClassFlags<wrl::ClassicCom>, IVBuffer>
{
public:
    HRESULT RuntimeClassInitialize(const VBUFFER_DESC* in_desc, const void* _in_data)
    {
        desc = *in_desc;
        if (desc.ByteWidth == 0) return E_INVALIDARG;
        data.resize(desc.ByteWidth);

        if (_in_data) std::copy((const uint8_t*)_in_data, (const uint8_t*)_in_data + desc.ByteWidth, data.begin());
        return S_OK;
    }
public:
    VBuffer() = default;
private:
    ~VBuffer() = default;
public:
    virtual void __stdcall GetDesc(VBUFFER_DESC* _out_Desc)override
    {
        *_out_Desc = desc;
    }
private:
    VBUFFER_DESC desc{0};
    std::vector<uint8_t> data;
};
