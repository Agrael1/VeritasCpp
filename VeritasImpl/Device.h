#pragma once
#include <Interfaces.h>

class VGraphicsDevice : public wrl::RuntimeClass<wrl::RuntimeClassFlags<wrl::ClassicCom>, IVDevice>
{
public:
	VGraphicsDevice() = default;
public:
	HRESULT __stdcall CreateBuffer(const VBUFFER_DESC* desc, IVBuffer** _out_Bufptr, const void* initialData = nullptr)override;
	HRESULT __stdcall CreateTexture2D(const VTEXTURE_DESC* desc, IVTexture** _out_texptr, const void* initialData = nullptr)override;
	HRESULT __stdcall CreateRenderTargetView(IVTexture* resource, VRTV_DESC* _out_rtv)const override;
	HRESULT __stdcall CreateDepthStencilView(IVTexture* resource, VDSV_DESC* _out_rtv)const override;
	HRESULT __stdcall CreateInputLayout(const VINPUT_ELEMENT* pInputElementDescs, uint32_t NumElements,
		const void* pShaderBytecodeWithInputSignature, uint32_t BytecodeLength, IVInputLayout** _out_InputLayout)override;
private:
	~VGraphicsDevice() = default;
};