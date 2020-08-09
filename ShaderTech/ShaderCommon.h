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
	virtual void __stdcall UpdateConstants(uint8_t* const* constants) = 0;
	virtual void __stdcall Invoke(const void* vs_in, void* _out_vertex) = 0;
	virtual void __stdcall GetByteCode(const char** _out_bytecode) = 0;
};

extern "C" HRESULT __stdcall VFMakeShader(const char* ShaderName, IVShader** _out_VSPtr);
