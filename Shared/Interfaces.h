#pragma once
#include <Descriptors.h>
#include <wrl.h>
#include "../ShaderTech/ShaderCommon.h"

namespace wrl = Microsoft::WRL;

// {979E8BE5-01F7-4264-99D1-7517ACD6B138}
DEFINE_GUID(IID_VBuffer,
    0x979e8be5, 0x1f7, 0x4264, 0x99, 0xd1, 0x75, 0x17, 0xac, 0xd6, 0xb1, 0x38);

MIDL_INTERFACE("979E8BE5-01F7-4264-99D1-7517ACD6B138")
IVBuffer : public IUnknown
{
    virtual void __stdcall GetDesc(VBUFFER_DESC * _out_Desc) = 0;
};

// {6A4326ED-4D1D-40EA-AC33-F0CE0C74FA13}
DEFINE_GUID(IID_VInputLayout,
	0x6a4326ed, 0x4d1d, 0x40ea, 0xac, 0x33, 0xf0, 0xce, 0xc, 0x74, 0xfa, 0x13);

MIDL_INTERFACE("6A4326ED-4D1D-40EA-AC33-F0CE0C74FA13")
IVInputLayout : public IUnknown
{
public:
};

// {71FBFB13-F224-47A4-853F-3A088B95D552}
DEFINE_GUID(IID_VTexture,
	0x71fbfb13, 0xf224, 0x47a4, 0x85, 0x3f, 0x3a, 0x8, 0x8b, 0x95, 0xd5, 0x52);

MIDL_INTERFACE("71FBFB13-F224-47A4-853F-3A088B95D552")
IVTexture : public IUnknown
{
	virtual void GetDesc(VTEXTURE_DESC * desc)const = 0;
};


// {0085FC95-F47C-47A2-99CE-A92E13471C05}
DEFINE_GUID(IID_VDevice,
	0x85fc95, 0xf47c, 0x47a2, 0x99, 0xce, 0xa9, 0x2e, 0x13, 0x47, 0x1c, 0x5);

MIDL_INTERFACE("0085FC95-F47C-47A2-99CE-A92E13471C05")
IVDevice : public IUnknown
{
	virtual HRESULT __stdcall CreateBuffer(const VBUFFER_DESC* desc, IVBuffer** _out_Bufptr, const void* initialData = nullptr) = 0;
	virtual HRESULT __stdcall CreateTexture2D(const VTEXTURE_DESC* desc, IVTexture** _out_texptr, const void* initialData = nullptr) = 0;
	virtual HRESULT __stdcall CreateRenderTargetView(IVTexture* resource, VRTV_DESC* _out_rtv) = 0;
	virtual HRESULT __stdcall CreateInputLayout(const VINPUT_ELEMENT* pInputElementDescs, uint32_t NumElements,
		const void* pShaderBytecodeWithInputSignature, uint32_t BytecodeLength, IVInputLayout** _out_InputLayout) = 0;
};

// {2CBC26A3-8408-4EF8-8B77-54CD55178D6F}
DEFINE_GUID(IID_VContext,
	0x2cbc26a3, 0x8408, 0x4ef8, 0x8b, 0x77, 0x54, 0xcd, 0x55, 0x17, 0x8d, 0x6f);

MIDL_INTERFACE("2CBC26A3-8408-4EF8-8B77-54CD55178D6F")
IVContext : public IUnknown
{
	virtual HRESULT __stdcall IASetPrimitiveTopology(VPRIMITIVE_TOPOLOGY Topology) = 0;
	virtual HRESULT __stdcall IASetVertexBuffers(uint32_t StartSlot, uint32_t NumBuffers, IVBuffer* const* ppVertexBuffers, const uint32_t* pStrides, const uint32_t* pOffsets) = 0;
	virtual HRESULT __stdcall IASetIndexBuffer(IVBuffer* indexBuffer, VFORMAT format, uint32_t offsetBytes) = 0;
	virtual HRESULT __stdcall IASetInputLayout(IVInputLayout* pInputLayout) = 0;
	virtual HRESULT __stdcall VSSetShader(IVShader* pVertexShader) = 0;
	virtual HRESULT __stdcall VSSetConstantBuffers(uint32_t StartSlot, uint32_t NumBuffers, IVBuffer* const* ppConstantBuffers) = 0;
	virtual HRESULT __stdcall RSSetViewports(uint32_t numVPs, const VVIEWPORT_DESC* _arr_VPs) = 0;
	virtual HRESULT __stdcall OMSetRenderTargets(uint32_t numViews, const VRTV_DESC* const _arr_RTVs) = 0;
	virtual HRESULT __stdcall ClearRenderTarget(VRTV_DESC* rtv, uint32_t col) = 0;
};


// {33B6E20B-3C6F-412A-AB6A-42580B911D2B}
DEFINE_GUID(IID_VSwapChain,
	0x33b6e20b, 0x3c6f, 0x412a, 0xab, 0x6a, 0x42, 0x58, 0xb, 0x91, 0x1d, 0x2b);

MIDL_INTERFACE("33B6E20B-3C6F-412A-AB6A-42580B911D2B")
IVSwapChain : public IUnknown
{
	virtual void __stdcall Present() = 0;
	virtual void __stdcall GetRenderTarget(uint32_t number, VRTV_DESC* _out_buf) = 0;
};


extern "C" HRESULT __stdcall VFCreateDevice(HWND window, IVDevice **_out_device, IVContext **_out_context);
extern "C" HRESULT __stdcall VFCreateSwapChain(VSWAP_CHAIN_DESC * descriptor, IVDevice * device, IVSwapChain * *_out_swapchain);