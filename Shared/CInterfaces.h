#pragma once
#include <Descriptors.h>
#include <stdint.h>
#include <WinSetup.h>

#define interface struct

typedef interface IUnknown IUnknown;
typedef interface IVBuffer IVBuffer;
typedef interface IVTexture IVTexture;
typedef interface IVInputLayout IVInputLayout;
typedef interface IVShader IVShader;
typedef interface IVDevice IVDevice;
typedef interface IVContext IVContext;

typedef struct IUnknownVtbl
{
    HRESULT(STDMETHODCALLTYPE* QueryInterface)(
        IUnknown* This,
        void* riid,
        void** ppvObject);

    uint32_t(STDMETHODCALLTYPE* AddRef)(
        IUnknown* This);

    uint32_t(STDMETHODCALLTYPE* Release)(
        IUnknown* This);
}IUnknownVtbl;
interface IUnknown
{
    const IUnknownVtbl* method;
};

typedef struct IVBufferVtbl
{
    IUnknownVtbl _unknwn;
    void(STDMETHODCALLTYPE* GetDesc)(
        const IVBuffer* This,
        VBUFFER_DESC* _out_Desc);
}IVBufferVtbl;
interface IVBuffer
{
    const IVBufferVtbl* method;
};

typedef struct IVTextureVtbl
{
    IUnknownVtbl _unknwn;
    void(STDMETHODCALLTYPE* GetDesc)(
        const IVTexture* This,
        VTEXTURE_DESC* _out_Desc);
}IVTextureVtbl;
interface IVTexture
{
    const IVTextureVtbl* method;
};

typedef struct IVInputLayoutVtbl
{
    IUnknownVtbl _unknwn;
}IVInputLayoutVtbl;
interface IVInputLayout
{
    const IVTextureVtbl* method;
};

typedef struct IVShaderVtbl
{
    IUnknownVtbl _unknwn;
    void(STDMETHODCALLTYPE* UpdateConstants)(
        IVShader* This,
        /* [in] */ void* constants[MaxBuffers]);

    void(STDMETHODCALLTYPE* Invoke)(
        IVShader* This,
        const void* _in_data,
        void* _out_data);

    void(STDMETHODCALLTYPE* GetMonotonicSize)(
        IVShader* This,
        uint32_t* _out_vsize);

    void(STDMETHODCALLTYPE* GetPositionIndex)(
        IVShader* This,
        uint32_t* _out_poscoord);
}IVShaderVtbl;
interface IVShader
{
    const struct IVShaderVtbl* method;
};

typedef struct IVDeviceVtbl
{
    IUnknownVtbl _unknwn;
    HRESULT(STDMETHODCALLTYPE* CreateBuffer)(
        IVDevice* This,
        /* [in] */ const VBUFFER_DESC* desc,
        /* [out] */ IVBuffer** _out_Bufptr,
        const void* initialData);

    HRESULT(STDMETHODCALLTYPE* CreateTexture2D)(
        IVDevice* This,
        /* [in] */ const VTEXTURE_DESC* desc,
        /* [out] */ IVTexture** _out_texptr,
        const void* initialData);

    HRESULT(STDMETHODCALLTYPE* CreateRenderTargetView)(
        IVDevice* This,
        /* [in] */ IVTexture* resource,
        /* [out] */ VRTV_DESC* _out_rtv);

    HRESULT(STDMETHODCALLTYPE* CreateDepthStencilView)(
        IVDevice* This,
        /* [in] */ IVTexture* resource,
        /* [out] */ VDSV_DESC* _out_rtv);

    HRESULT(STDMETHODCALLTYPE* CreateInputLayout)(
        IVDevice* This,
        const VINPUT_ELEMENT* pInputElementDescs,
        uint32_t NumElements,
        const void* pShaderBytecodeWithInputSignature,
        uint32_t BytecodeLength,
        /* [out] */ IVInputLayout** _out_InputLayout);
} IVDeviceVtbl;
interface IVDevice
{
    const struct IVDeviceVtbl* method;
};

typedef struct IVContextVtbl
{
    IUnknownVtbl _unknwn;

    HRESULT(STDMETHODCALLTYPE* IASetPrimitiveTopology)(
        IVContext* This,
        VPRIMITIVE_TOPOLOGY Topology);

    HRESULT(STDMETHODCALLTYPE* IASetVertexBuffers)(
        IVContext* This,
        uint32_t StartSlot,
        uint32_t NumBuffers,
        IVBuffer* const* ppVertexBuffers,
        const uint32_t* pStrides,
        const uint32_t* pOffsets);

    HRESULT(STDMETHODCALLTYPE* IASetIndexBuffer)(
        IVContext* This,
        IVBuffer* indexBuffer,
        VFORMAT format,
        uint32_t offsetBytes);

    HRESULT(STDMETHODCALLTYPE* IASetInputLayout)(
        IVContext* This,
        IVInputLayout* pInputLayout);

    HRESULT(STDMETHODCALLTYPE* VSSetShader)(
        IVContext* This,
        IVShader* pVertexShader);

    HRESULT(STDMETHODCALLTYPE* VSSetConstantBuffers)(
        IVContext* This,
        uint32_t StartSlot,
        uint32_t NumBuffers,
        IVBuffer* const* ppConstantBuffers);

    HRESULT(STDMETHODCALLTYPE* PSSetShader)(
        IVContext* This,
        IVShader* pVertexShader);

    HRESULT(STDMETHODCALLTYPE* PSSetConstantBuffers)(
        IVContext* This,
        uint32_t StartSlot,
        uint32_t NumBuffers,
        IVBuffer* const* ppConstantBuffers);

    HRESULT(STDMETHODCALLTYPE* RSSetViewport)(
        IVContext* This,
        uint32_t numVPs,
        const VVIEWPORT_DESC* pVP);

    HRESULT(STDMETHODCALLTYPE* OMSetRenderTargets)(
        IVContext* This,
        uint32_t numViews,
        const VRTV_DESC* const _arr_RTVs);

    HRESULT(STDMETHODCALLTYPE* OMSetDepthStencil)(
        IVContext* This,
        const VDSV_DESC* DSV);

    HRESULT(STDMETHODCALLTYPE* ClearRenderTarget)(
        IVContext* This,
        VRTV_DESC* rtv,
        uint32_t col);

    HRESULT(STDMETHODCALLTYPE* ClearDepthStencil)(
        IVContext* This,
        VDSV_DESC* dsv,
        float value);

    HRESULT(STDMETHODCALLTYPE* Map)(
        IVContext* This,
        IVBuffer* pResource,
        VMAPPED_SUBRESOURCE* _out_pMappedResource);

    HRESULT(STDMETHODCALLTYPE* Unmap)(
        IVContext* This,
        IVBuffer* pResource);

    HRESULT(STDMETHODCALLTYPE* DrawIndexed)(
        IVContext* This,
        uint32_t nofVertices);

    HRESULT(STDMETHODCALLTYPE* Draw)(
        IVContext* This,
        uint32_t nofVertices);
}IVContextVtbl;
interface IVContext
{
    const IVContextVtbl* method;
};