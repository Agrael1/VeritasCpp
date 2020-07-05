#pragma once
#include <Framework\GDISetup.h>
#include <DirectXPackedVector.h>
#include <wil\wrl.h>
#include <Engine\Descriptors.h>
#include <span>
#include <memory>

namespace wrl = Microsoft::WRL;


struct VTEXTURE_DESC
{
	uint32_t Width;
	uint32_t Height;
	PixelFormat PixelFormat;
	uint32_t BindFlags;
};

// {71FBFB13-F224-47A4-853F-3A088B95D552}
DEFINE_GUID(IID_IVTexture,
	0x71fbfb13, 0xf224, 0x47a4, 0x85, 0x3f, 0x3a, 0x8, 0x8b, 0x95, 0xd5, 0x52);

MIDL_INTERFACE("71FBFB13-F224-47A4-853F-3A088B95D552")
IVTexture : public IUnknown
{
	virtual void GetDesc(VTEXTURE_DESC* desc)const noexcept = 0;
};

class VTexture : public wrl::RuntimeClass<wrl::RuntimeClassFlags<wrl::ClassicCom>, IVTexture>
{
	friend class VSwapChain;
	friend class VGraphicsDevice;
public:
	HRESULT RuntimeClassInitialize(const VTEXTURE_DESC* in_desc, const void* _in_data = nullptr)
	{
		desc = *in_desc;
		if (!desc.Height||!desc.Width) return E_INVALIDARG;
		data = std::make_unique<DirectX::PackedVector::XMCOLOR[]>(in_desc->Width * in_desc->Height);

		if (_in_data) std::copy((const uint8_t*)_in_data, (const uint8_t*)_in_data + desc.Width * desc.Height, data.get());
		return S_OK;
	}
public:
	VTexture() = default;
public:
	virtual void GetDesc(VTEXTURE_DESC* _out_desc)const noexcept override
	{
		*_out_desc = desc;
	}
private:
	void GetView(Gdiplus::BitmapData* in)noexcept
	{
		(in)->Width = desc.Width;
		(in)->Height = desc.Height;
		(in)->PixelFormat = desc.PixelFormat;
		(in)->Stride = sizeof(DirectX::PackedVector::XMCOLOR);
		(in)->Scan0 = data.get();
	}
private:
	VTEXTURE_DESC desc{0};
	std::unique_ptr<DirectX::PackedVector::XMCOLOR[]> data{0};
};


using RenderTargetView = Gdiplus::BitmapData;