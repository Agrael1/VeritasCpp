#pragma once
#include <Descriptors.h>
#include <stdint.h>
#include <WinSetup.h>

typedef const struct _IUnknown_vtbl     IUnknown_vtbl,    *IUnknown;
typedef const struct _IVBuffer_vtbl     IVBuffer_vtbl,    *IVBuffer;
typedef const struct _IVTexture_vtbl    IVTexture_vtbl,   *IVTexture;
typedef const struct _IVDevice_vtbl     IVDevice_vtbl,    *IVDevice;
typedef const struct _IVInputLayout_vtbl     IVInputLayout_vtbl,    *IVInputLayout;

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
struct _IVTexture_vtbl
{
    IUnknown_vtbl _unknwn;
    void(__stdcall* GetDesc)(
        const IVTexture* This,
        VTEXTURE_DESC* _out_Desc);
};
struct _IVInputLayout_vtbl
{
    IUnknown_vtbl _unknwn;
};

struct _IVDevice_vtbl
{
    IUnknown_vtbl _unknwn;
    HRESULT (__stdcall* CreateBuffer)(IVDevice* This, const VBUFFER_DESC* desc, IVBuffer** _out_Bufptr, const void* initialData);
    HRESULT (__stdcall* CreateTexture2D)(IVDevice* This, const VTEXTURE_DESC* desc, IVTexture** _out_texptr, const void* initialData);
    HRESULT (__stdcall* CreateRenderTargetView)(IVDevice* This, IVTexture* resource, VRTV_DESC* _out_rtv);
    HRESULT (__stdcall* CreateDepthStencilView)(IVDevice* This, IVTexture* resource, VDSV_DESC* _out_rtv);
    HRESULT (__stdcall* CreateInputLayout)(IVDevice* This, const VINPUT_ELEMENT* pInputElementDescs, uint32_t NumElements,
        const void* pShaderBytecodeWithInputSignature, uint32_t BytecodeLength, IVInputLayout** _out_InputLayout);
};