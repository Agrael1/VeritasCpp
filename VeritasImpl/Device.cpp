#include "Device.h"
#include <GdiException.h>
#include "Buffer.h"
#include "Texture.h"
#include "InputLayout.h"

GDIPlusManager gdipm;

HRESULT VGraphicsDevice::RuntimeClassInitialize(HWND window, bool ICCColorAdjustment)
{
	HWND hwnd = window;

	if (ICCColorAdjustment)
	{
		GDI_CALL_EXCEPT(Gdiplus::DllExports::GdipCreateFromHWNDICM(hwnd, &pGfx));
	}
	else
	{
		GDI_CALL_EXCEPT(Gdiplus::DllExports::GdipCreateFromHWND(hwnd, &pGfx));
	}
	return S_OK;
}

HRESULT __stdcall VGraphicsDevice::CreateBuffer(const VBUFFER_DESC* desc, IVBuffer** _out_Bufptr, const void* initialData)
{
	return wrl::MakeAndInitialize<VBuffer>(_out_Bufptr, desc, initialData);
}
HRESULT __stdcall VGraphicsDevice::CreateTexture2D(const VTEXTURE_DESC* desc, IVTexture** _out_texptr, const void* initialData)
{
	return wrl::MakeAndInitialize<VTexture>(_out_texptr, desc, initialData);
}
HRESULT __stdcall VGraphicsDevice::CreateRenderTargetView(IVTexture* resource, VRTV_DESC* _out_rtv)const
{
	static_cast<VTexture*>(resource)->GetView(_out_rtv);
	return S_OK;
}
HRESULT __stdcall VGraphicsDevice::CreateDepthStencilView(IVTexture* resource, VDSV_DESC* _out_rtv)const
{
	static_cast<VTexture*>(resource)->GetView(_out_rtv);
	return S_OK;
}
HRESULT __stdcall VGraphicsDevice::CreateInputLayout(const VINPUT_ELEMENT* pInputElementDescs, uint32_t NumElements, const void* pShaderBytecodeWithInputSignature, uint32_t BytecodeLength, IVInputLayout** _out_InputLayout)
{
	return wrl::MakeAndInitialize<VInputLayout>(_out_InputLayout, pInputElementDescs, NumElements);
}

Gdiplus::GpGraphics* VGraphicsDevice::GetRawGraphics()const noexcept
{
	return pGfx.get();
}