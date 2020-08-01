#pragma once
#include <Framework\GDIPlusManager.h>
#include <wil\include\wil\win32_helpers.h>
#include <Interfaces.h>

class VGraphicsDevice: public wrl::RuntimeClass<wrl::RuntimeClassFlags<wrl::ClassicCom>, IVDevice>
{
	friend class VSwapChain;
	using unique_gdi = wil::unique_any<Gdiplus::GpGraphics*, decltype(&Gdiplus::DllExports::GdipDeleteGraphics), Gdiplus::DllExports::GdipDeleteGraphics>;
public:
	VGraphicsDevice() = default;
public:
	HRESULT RuntimeClassInitialize(HWND window, bool ICCColorAdjustment = false);
public:
	HRESULT __stdcall CreateBuffer(const VBUFFER_DESC* desc, IVBuffer** _out_Bufptr, const void* initialData = nullptr)override;
	HRESULT __stdcall CreateTexture2D(const VTEXTURE_DESC* desc, IVTexture** _out_texptr, const void* initialData = nullptr)override;
	HRESULT __stdcall CreateRenderTargetView(IVTexture* resource, VRTV_DESC* _out_rtv)override;
	HRESULT __stdcall CreateInputLayout(const VINPUT_ELEMENT* pInputElementDescs, uint32_t NumElements,
		const void* pShaderBytecodeWithInputSignature, uint32_t BytecodeLength, IVInputLayout** _out_InputLayout)override;
private:
	~VGraphicsDevice() = default;
private:
	Gdiplus::GpGraphics* GetRawGraphics()noexcept;
private:
	unique_gdi pGfx;
};

