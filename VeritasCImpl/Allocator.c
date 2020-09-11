#include "Allocator.h"

typedef struct
{
    IUnknown vtbl;
    uint32_t __internal_refcount;
}DummyCOM;

HRESULT __stdcall QueryInterface(
    IUnknown* This,
    void* riid,
    void** ppvObject)
{
    return E_NOTIMPL;
}
uint32_t __stdcall AddRef(IUnknown* This)
{
    return ++((DummyCOM*)This)->__internal_refcount;
}
uint32_t __stdcall Release(IUnknown* This)
{
    uint32_t pval = --((DummyCOM*)This)->__internal_refcount;
    if (!pval)
    {
        Allocator* al = (Allocator*)((unsigned char*)This - sizeof(void*));
        if (al->vdtor)
            al->vdtor(This);
        free(al);
    }
    return pval;
}