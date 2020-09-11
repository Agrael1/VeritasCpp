#define BUFFER_IMPL
#include "Buffer.h"
#include <malloc.h>
#include <memory.h>

void __stdcall GetDesc(const struct VBuffer* This, VBUFFER_DESC* _out_Desc)
{
    *_out_Desc = This->desc;
}

HRESULT RuntimeClassInitialize(struct VBuffer* This, const VBUFFER_DESC* _in_desc, const void* _in_data)
{
    This->desc = *_in_desc;
    if (This->desc.ByteWidth == 0) return E_INVALIDARG;
    This->data = malloc(This->desc.ByteWidth);

    if (_in_data)
        memcpy_s(This->data, This->desc.ByteWidth, (const uint8_t*)_in_data, This->desc.ByteWidth);

    return S_OK;
}
void RuntimeClassDestroy(struct VBuffer* This)
{
    if (This->data) free(This->data);
}

VirtualTable(IVBuffer)
{
    ._unknwn = __COM_IUnknown,
    .GetDesc = GetDesc
};

ENDCLASSDESC