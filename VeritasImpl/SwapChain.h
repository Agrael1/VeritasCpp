#pragma once
#include <GdiException.h>
#include <Interfaces.h>
#include <wil\win32_helpers.h>
#include <optional>
#include "Texture.h"

class VSwapChain : public wrl::RuntimeClass<wrl::RuntimeClassFlags<wrl::ClassicCom>, IVSwapChain>
{
	class VFrame
	{
		using unique_image = wil::unique_any<Gdiplus::GpBitmap*, decltype(&Gdiplus::DllExports::GdipDisposeImage), Gdiplus::DllExports::GdipDisposeImage>;
		using unique_cached_bitmap = wil::unique_any<Gdiplus::GpCachedBitmap*, decltype(&Gdiplus::DllExports::GdipDeleteCachedBitmap), Gdiplus::DllExports::GdipDeleteCachedBitmap>;
	public:
		VFrame(int width, int height, Gdiplus::GpGraphics* in_target);
	public:
		void LockImage(VRTV_DESC& _out_view, Gdiplus::Rect lockArea, Gdiplus::ImageLockMode mode);
		void LockFullImage(VRTV_DESC& _out_view, Gdiplus::ImageLockMode mode);
		void UnlockImage(VRTV_DESC& view);
		void Draw()const;
		VPIXEL_FORMAT GetPixelFormat()const noexcept;
	private:
		unique_image image;
		mutable unique_cached_bitmap output;
		Gdiplus::GpGraphics* target;
		Gdiplus::Rect frameArea;
		VPIXEL_FORMAT format = VPIXEL_FORMAT::PARGB32bpp;
	};
public:
	VSwapChain() = default;
public:
	HRESULT RuntimeClassInitialize(const VSWAP_CHAIN_DESC* desc, const IVDevice* gfx);
public:
	void __stdcall Present()override;
	void __stdcall GetRenderTarget(uint32_t number, VRTV_DESC* _out_buf)override;
private:
	~VSwapChain() = default;
private:
	std::optional<VFrame> Frame;
	VRTV_DESC BackBuffer{ 0 };
	wrl::ComPtr<VTexture> RenderBuffer;
};