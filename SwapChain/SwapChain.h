#pragma once
#include "GDISetup.h"
#include <Interfaces.h>
#include <wil\win32_helpers.h>


class VSwapChain : public wrl::RuntimeClass<wrl::RuntimeClassFlags<wrl::ClassicCom>, IVSwapChain>
{
	class VFrame
	{
		using unique_gdi = wil::unique_any<Gdiplus::GpGraphics*, decltype(&Gdiplus::DllExports::GdipDeleteGraphics), Gdiplus::DllExports::GdipDeleteGraphics>;
		using unique_image = wil::unique_any<Gdiplus::GpBitmap*, decltype(&Gdiplus::DllExports::GdipDisposeImage), Gdiplus::DllExports::GdipDisposeImage>;
		using unique_cached_bitmap = wil::unique_any<Gdiplus::GpCachedBitmap*, decltype(&Gdiplus::DllExports::GdipDeleteCachedBitmap), Gdiplus::DllExports::GdipDeleteCachedBitmap>;
	public:
		VFrame(int width, int height, HWND hwnd, bool ICCColorAdjustment = false);
	public:
		void LockImage(VRTV_DESC& _out_view, Gdiplus::Rect lockArea, Gdiplus::ImageLockMode mode);
		void LockFullImage(VRTV_DESC& _out_view, Gdiplus::ImageLockMode mode);
		void UnlockImage(VRTV_DESC& view);
		void Draw()const;
		VPIXEL_FORMAT GetPixelFormat()const noexcept;
	private:
		mutable unique_cached_bitmap output;
		unique_image image;
		unique_gdi pGfx;
		Gdiplus::Rect frameArea;
		VPIXEL_FORMAT format = VPIXEL_FORMAT::PARGB32bpp;
	};
public:
	VSwapChain(const VSWAP_CHAIN_DESC* desc, IVDevice* gfx, HWND hwnd);
public:
	void __stdcall Present()override;
	void __stdcall GetRenderTarget(uint32_t number, VRTV_DESC* _out_buf)override;
private:
	~VSwapChain() = default;
private:
	VFrame Frame;
	VRTV_DESC BackBuffer{ 0 };
	wrl::ComPtr<IVTexture> RenderBuffer;
};