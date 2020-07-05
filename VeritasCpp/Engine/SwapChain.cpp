#include "SwapChain.h"
#include <Framework\GdiException.h>
#include <algorithm>

VSwapChain::VFrame::VFrame(int width, int height, Gdiplus::GpGraphics* in_target)noxnd
	:target(in_target), frameArea(0, 0, width, height)
{
	GDI_CALL_EXCEPT(Gdiplus::DllExports::GdipCreateBitmapFromGraphics(width,
		height,
		target,
		&image));

	GDI_CALL_EXCEPT(Gdiplus::DllExports::GdipGetImagePixelFormat(image.get(), reinterpret_cast<Gdiplus::PixelFormat*>(&format)));
}

void VSwapChain::VFrame::LockImage(RenderTargetView& _out_view, Gdiplus::Rect lockArea, Gdiplus::ImageLockMode mode)noxnd
{
	GDI_CALL_EXCEPT(Gdiplus::DllExports::GdipBitmapLockBits(
		image.get(),
		&lockArea,
		mode | Gdiplus::ImageLockModeUserInputBuf,
		format,
		&_out_view));
}
void VSwapChain::VFrame::LockFullImage(RenderTargetView& _out_view, Gdiplus::ImageLockMode mode)noxnd
{
	GDI_CALL_EXCEPT(Gdiplus::DllExports::GdipBitmapLockBits(
		image.get(),
		&frameArea,
		mode | Gdiplus::ImageLockModeUserInputBuf,
		format,
		&_out_view));
}
void VSwapChain::VFrame::UnlockImage(RenderTargetView& view)noxnd
{
	GDI_CALL_EXCEPT(Gdiplus::DllExports::GdipBitmapUnlockBits(
		image.get(),
		&view));
}
void VSwapChain::VFrame::Draw() const noxnd
{
	GDI_CALL_EXCEPT(Gdiplus::DllExports::GdipCreateCachedBitmap(
		image.get(),
		target,
		&output
	));

	GDI_CALL_EXCEPT(Gdiplus::DllExports::GdipDrawCachedBitmap(
		target,
		output.get(),
		0, 0
	));
}
PixelFormat VSwapChain::VFrame::GetPixelFormat() const noexcept
{
	return format;
}


VSwapChain::VSwapChain(int width, int height, VGraphicsDevice& gfx)noxnd
	:Frame(width, height, gfx.GetRawGraphics())
{
	VTEXTURE_DESC vtx;
	vtx.Width = width;
	vtx.Height = height;
	vtx.BindFlags = VBIND_FLAG::RENDER_TARGET;
	vtx.PixelFormat = Frame.GetPixelFormat();

	wrl::MakeAndInitialize<VTexture>(&RenderBuffer, &vtx);
	gfx.CreateRenderTargetView(RenderBuffer.Get(), &BackBuffer);

	Frame.LockFullImage(BackBuffer, Gdiplus::ImageLockModeWrite);
}

void VSwapChain::Present()noxnd
{
	Frame.UnlockImage(BackBuffer);
	Frame.Draw();
	Frame.LockFullImage(BackBuffer, Gdiplus::ImageLockModeWrite);
}

