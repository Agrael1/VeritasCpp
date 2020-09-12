#include "SwapChain.h"
#include "GdiException.h"

import "GDIPlusManager.h";

GDIPlusManager gdipm;

VSwapChain::VFrame::VFrame(int width, int height, HWND hwnd, bool ICCColorAdjustment)
	: frameArea(0, 0, width, height)
{
	if (ICCColorAdjustment)
	{
		GDI_CALL_EXCEPT(Gdiplus::DllExports::GdipCreateFromHWNDICM(hwnd, &pGfx));
	}
	else
	{
		GDI_CALL_EXCEPT(Gdiplus::DllExports::GdipCreateFromHWND(hwnd, &pGfx));
	}

	GDI_CALL_EXCEPT(Gdiplus::DllExports::GdipCreateBitmapFromGraphics(width,
		height,
		pGfx.get(),
		&image));

	GDI_CALL_EXCEPT(Gdiplus::DllExports::GdipGetImagePixelFormat(image.get(), reinterpret_cast<Gdiplus::PixelFormat*>(&format)));
}


void VSwapChain::VFrame::LockImage(VRTV_DESC& _out_view, Gdiplus::Rect lockArea, Gdiplus::ImageLockMode mode)
{
	GDI_CALL_EXCEPT(Gdiplus::DllExports::GdipBitmapLockBits(
		image.get(),
		&lockArea,
		mode | Gdiplus::ImageLockModeUserInputBuf,
		format,
		(Gdiplus::BitmapData*) & _out_view));
}
void VSwapChain::VFrame::LockFullImage(VRTV_DESC& _out_view, Gdiplus::ImageLockMode mode)
{
	GDI_CALL_EXCEPT(Gdiplus::DllExports::GdipBitmapLockBits(
		image.get(),
		&frameArea,
		mode | Gdiplus::ImageLockModeUserInputBuf,
		format,
		(Gdiplus::BitmapData*) & _out_view));
}
void VSwapChain::VFrame::UnlockImage(VRTV_DESC& view)
{
	GDI_CALL_EXCEPT(Gdiplus::DllExports::GdipBitmapUnlockBits(
		image.get(),
		(Gdiplus::BitmapData*) & view));
}
void VSwapChain::VFrame::Draw() const
{
	GDI_CALL_EXCEPT(Gdiplus::DllExports::GdipCreateCachedBitmap(
		image.get(),
		pGfx.get(),
		&output
	));

	GDI_CALL_EXCEPT(Gdiplus::DllExports::GdipDrawCachedBitmap(
		pGfx.get(),
		output.get(),
		0, 0
	));
}
VPIXEL_FORMAT VSwapChain::VFrame::GetPixelFormat() const noexcept
{
	return format;
}


VSwapChain::VSwapChain(const VSWAP_CHAIN_DESC* desc, IVDevice* gfx, HWND hwnd)
	:Frame(desc->Width, desc->Height, hwnd)
{
	VTEXTURE_DESC vtx;
	vtx.Width = desc->Width;
	vtx.Height = desc->Height;
	vtx.BindFlags = VBIND_FLAG::RENDER_TARGET;
	vtx.PixelFormat = Frame.GetPixelFormat();

	gfx->CreateTexture2D(&vtx, &RenderBuffer);
	gfx->CreateRenderTargetView(RenderBuffer.Get(), &BackBuffer);

	Frame.LockFullImage(BackBuffer, Gdiplus::ImageLockModeWrite);
}

void VSwapChain::GetRenderTarget(uint32_t number, VRTV_DESC* _out_buf)
{
	*_out_buf = BackBuffer;
}
void VSwapChain::Present()
{
	Frame.UnlockImage(BackBuffer);
	Frame.Draw();
	Frame.LockFullImage(BackBuffer, Gdiplus::ImageLockModeWrite);
}

HRESULT __stdcall VFCreateSwapChain(const VSWAP_CHAIN_DESC* descriptor, IVDevice* device, HWND window, IVSwapChain** _out_swapchain)
{
	return wrl::Make<VSwapChain>(descriptor, device, window).CopyTo(_out_swapchain);
}
