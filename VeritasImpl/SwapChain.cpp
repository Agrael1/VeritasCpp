#include "SwapChain.h"
#include "Device.h"


VSwapChain::VFrame::VFrame(int width, int height, Gdiplus::GpGraphics* in_target)
	:target(in_target), frameArea(0, 0, width, height)
{
	GDI_CALL_EXCEPT(Gdiplus::DllExports::GdipCreateBitmapFromGraphics(width,
		height,
		target,
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
		target,
		&output
	));

	GDI_CALL_EXCEPT(Gdiplus::DllExports::GdipDrawCachedBitmap(
		target,
		output.get(),
		0, 0
	));
}
VPIXEL_FORMAT VSwapChain::VFrame::GetPixelFormat() const noexcept
{
	return format;
}


HRESULT VSwapChain::RuntimeClassInitialize(const VSWAP_CHAIN_DESC* desc, const IVDevice* gfx)
{
	Frame.emplace(desc->Width, desc->Height, static_cast<const VGraphicsDevice*>(gfx)->GetRawGraphics());

	VTEXTURE_DESC vtx;
	vtx.Width = desc->Width;
	vtx.Height = desc->Height;
	vtx.BindFlags = VBIND_FLAG::RENDER_TARGET;
	vtx.PixelFormat = Frame->GetPixelFormat();

	wrl::MakeAndInitialize<VTexture>(&RenderBuffer, &vtx);
	gfx->CreateRenderTargetView(RenderBuffer.Get(), &BackBuffer);

	Frame->LockFullImage(BackBuffer, Gdiplus::ImageLockModeWrite);
	return S_OK;
}

void VSwapChain::GetRenderTarget(uint32_t number, VRTV_DESC* _out_buf)
{
	*_out_buf = BackBuffer;
}
void VSwapChain::Present()
{
	Frame->UnlockImage(BackBuffer);
	Frame->Draw();
	Frame->LockFullImage(BackBuffer, Gdiplus::ImageLockModeWrite);
}
