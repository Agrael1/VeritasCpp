#include "Graphics.h"
#include "Window.h"
#include <Framework\GdiException.h>


GDIPlusManager gdipm;

VGraphicsDevice::VGraphicsDevice(Window& window, bool ICCColorAdjustment)
{
	HWND hwnd = window.GetWindowHandle();

	if (ICCColorAdjustment)
	{
		GDI_CALL_EXCEPT(Gdiplus::DllExports::GdipCreateFromHWNDICM(hwnd, &pGfx));
	}
	else
	{
		GDI_CALL_EXCEPT(Gdiplus::DllExports::GdipCreateFromHWND(hwnd, &pGfx));
	}
}

void VGraphicsDevice::CreateBuffer(const VBUFFER_DESC* desc, IVBuffer** _out_Bufptr, const void* initialData) noexcept(!_DEBUG)
{
	wrl::MakeAndInitialize<VBuffer>(_out_Bufptr, desc, initialData);
}
void VGraphicsDevice::CreateTexture2D(const VTEXTURE_DESC* desc, IVTexture** _out_texptr, const void* initialData) noexcept(!_DEBUG)
{
	wrl::MakeAndInitialize<VTexture>(_out_texptr, desc, initialData);
}
void VGraphicsDevice::CreateRenderTargetView(IVTexture* resource, RenderTargetView* _out_rtv) noexcept(!_DEBUG)
{
	static_cast<VTexture*>(resource)->GetView(_out_rtv);
}

Gdiplus::GpGraphics* VGraphicsDevice::GetRawGraphics() noexcept
{
	return pGfx.get();
}
