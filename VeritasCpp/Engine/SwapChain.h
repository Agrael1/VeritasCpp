#pragma once
#include <Framework\GDIPlusManager.h>
#include <DirectXPackedVector.h>
#include <wil\resource.h>
#include <Framework\ConditionalNoexcept.h>
#include <Engine\Graphics.h>
#include <RenderTarget.h>

class VSwapChain
{
	class VFrame
	{
		using unique_image = wil::unique_any<Gdiplus::GpBitmap*, decltype(&Gdiplus::DllExports::GdipDisposeImage), Gdiplus::DllExports::GdipDisposeImage>;
		using unique_cached_bitmap = wil::unique_any<Gdiplus::GpCachedBitmap*, decltype(&Gdiplus::DllExports::GdipDeleteCachedBitmap), Gdiplus::DllExports::GdipDeleteCachedBitmap>;
	public:
		VFrame(int width, int height, Gdiplus::GpGraphics* in_target)noxnd;
	public:
		void LockImage(RenderTargetView& _out_view, Gdiplus::Rect lockArea, Gdiplus::ImageLockMode mode)noxnd;
		void LockFullImage(RenderTargetView& _out_view, Gdiplus::ImageLockMode mode)noxnd;
		void UnlockImage(RenderTargetView& view)noxnd;
		void Draw()const noxnd;
		PixelFormat GetPixelFormat()const noexcept;
	private:
		unique_image image;
		mutable unique_cached_bitmap output;
		Gdiplus::GpGraphics* target;
		Gdiplus::Rect frameArea;
		PixelFormat format;
	};
public:
	VSwapChain(int width, int height, VGraphicsDevice& gfx)noxnd;
public:
	void Present()noxnd;
	void GetRenderTarget(uint32_t number, RenderTargetView* _out_buf)
	{
		*_out_buf = BackBuffer;
	}
private:
	VFrame Frame;
	RenderTargetView BackBuffer;
	wrl::ComPtr<VTexture> RenderBuffer;
};