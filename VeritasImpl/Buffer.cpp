#include "Buffer.h"

HRESULT VBuffer::RuntimeClassInitialize(const VBUFFER_DESC* in_desc, const void* _in_data)
{
    desc = *in_desc;
    if (desc.ByteWidth == 0) return E_INVALIDARG;
    data.resize(desc.ByteWidth);

    if (_in_data) 
        std::copy((const uint8_t*)_in_data, (const uint8_t*)_in_data + desc.ByteWidth, data.begin());

    return S_OK;
}

void __stdcall VBuffer::GetDesc(VBUFFER_DESC* _out_Desc)
{
    *_out_Desc = desc;
}