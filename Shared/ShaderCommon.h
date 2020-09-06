#pragma once
#include <cstdint>
#include <wrl.h>

namespace wrl = Microsoft::WRL;

// {238B3FF9-0BCB-4706-9237-E4632DA5E5C9}
DEFINE_GUID(IID_VShader,
	0x238b3ff9, 0x0bcb, 0x4706, 0x92, 0x37, 0xe4, 0x63, 0x2d, 0xa5, 0xe5, 0xc9);

MIDL_INTERFACE("238B3FF9-0BCB-4706-9237-E4632DA5E5C9")
IVShader : public IUnknown
{
	virtual void __stdcall UpdateConstants(void* const* constants) = 0;
	virtual void __stdcall Invoke(const void* _in_data, void* _out_data) = 0;
	virtual void __stdcall GetMonotonicSize(uint32_t* _out_vsize) = 0;
};

extern "C" HRESULT __stdcall VFMakeShader(const char* ShaderName, IVShader * *_out_VSPtr);