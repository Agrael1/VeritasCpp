#pragma once
#include <Descriptors.h>
#include <stdint.h>
#include <WinSetup.h>

typedef const struct _IUnknown_vtbl IUnknown_vtbl,* IUnknown;
typedef const struct _IVBuffer_vtbl IVBuffer_vtbl,* IVBuffer;


struct _IUnknown_vtbl
{
    HRESULT(__stdcall* QueryInterface)(
        IUnknown* This,
        void* riid,
        void** ppvObject);

    uint32_t(__stdcall* AddRef)(
        IUnknown* This);

    uint32_t(__stdcall* Release)(
        IUnknown* This);
};
struct _IVBuffer_vtbl
{
    IUnknown_vtbl _unknwn;
    void(__stdcall* GetDesc)(
        const IVBuffer* This,
        VBUFFER_DESC* _out_Desc);
};