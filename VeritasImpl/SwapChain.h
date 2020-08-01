#pragma once
#include "GDISetup.h"
#include "ConditionalNoexcept.h"
#include <Interfaces.h>
#include <Framework\wil\include\wil\win32_helpers.h>
#include <optional>

#include "Texture.h"

class VSwapChain : public wrl::RuntimeClass<wrl::RuntimeClassFlags<wrl::ClassicCom>, IVSwapChain>
{
	class VFrame
	{
		using unique_image = wil::unique_any<Gdiplus::GpBitmap*, decltype(&Gdiplus::DllExports::GdipDisposeImage), Gdiplus::DllExports::GdipDisposeImage>;
		using unique_cached_bitmap = wil::unique_any<Gdiplus::GpCachedBitmap*, decltype(&Gdiplus::DllExports::GdipDeleteCachedBitmap), Gdiplus::DllExports::GdipDeleteCachedBitmap>;
	public:
		VFrame(int width, int height, Gdiplus::GpGraphics* in_target)noxnd;
	public:
		void LockImage(VRTV_DESC& _out_view, Gdiplus::Rect lockArea, Gdiplus::ImageLockMode mode)noxnd;
		void LockFullImage(VRTV_DESC& _out_view, Gdiplus::ImageLockMode mode)noxnd;
		void UnlockImage(VRTV_DESC& view)noxnd;
		void Draw()const noxnd;
		VPIXEL_FORMAT GetPixelFormat()const noexcept;
	private:
		unique_image image;
		mutable unique_cached_bitmap output;
		Gdiplus::GpGraphics* target;
		Gdiplus::Rect frameArea;
		VPIXEL_FORMAT format;
	};
public:
	VSwapChain() = default;
public:
	HRESULT RuntimeClassInitialize(VSWAP_CHAIN_DESC* desc, IVDevice* gfx);
public:
	void Present()override;
	void GetRenderTarget(uint32_t number, VRTV_DESC* _out_buf)override;
private:
	~VSwapChain() = default;
private:
	std::optional<VFrame> Frame;
	VRTV_DESC BackBuffer;
	wrl::ComPtr<VTexture> RenderBuffer;
};

