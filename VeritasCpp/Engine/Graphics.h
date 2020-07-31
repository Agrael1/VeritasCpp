#pragma once
#include <Framework\GDIPlusManager.h>
#include <wil\resource.h>
#include <Framework\ConditionalNoexcept.h>
#include "Buffer.h"
#include "RenderTarget.h"
#include "InputLayout.h"

class VGraphicsDevice
{
	friend class VSwapChain;
	using unique_gdi = wil::unique_any<Gdiplus::GpGraphics*, decltype(&Gdiplus::DllExports::GdipDeleteGraphics), Gdiplus::DllExports::GdipDeleteGraphics>;
public:
	VGraphicsDevice(class Window& window, bool ICCColorAdjustment = false);
public:
	void CreateBuffer(const VBUFFER_DESC* desc, IVBuffer** _out_Bufptr, const void* initialData = nullptr)noxnd;
	void CreateTexture2D(const VTEXTURE_DESC* desc, IVTexture** _out_texptr, const void* initialData = nullptr)noxnd;
	void CreateRenderTargetView(IVTexture* resource, RenderTargetView* _out_rtv)noxnd;
	void CreateInputLayout(const VINPUT_ELEMENT* pInputElementDescs, uint32_t NumElements,
		const void* pShaderBytecodeWithInputSignature, uint32_t BytecodeLength, IVInputLayout** _out_InputLayout);
private:
	Gdiplus::GpGraphics* GetRawGraphics()noexcept;
private:
	unique_gdi pGfx;
};