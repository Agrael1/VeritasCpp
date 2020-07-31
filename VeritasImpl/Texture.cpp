#include "Texture.h"

HRESULT VTexture::RuntimeClassInitialize(const VTEXTURE_DESC* in_desc, const void* _in_data)
{
	desc = *in_desc;
	if (!desc.Height || !desc.Width) return E_INVALIDARG;
	data = std::make_unique<DirectX::PackedVector::XMCOLOR[]>(in_desc->Width * in_desc->Height);

	if (_in_data) std::copy((const uint8_t*)_in_data, (const uint8_t*)_in_data + desc.Width * desc.Height, data.get());
	return S_OK;
}

void VTexture::GetDesc(VTEXTURE_DESC* _out_desc) const
{
	*_out_desc = desc;
}

void VTexture::GetView(VRTV_DESC* in) noexcept
{
	(in)->Width = desc.Width;
	(in)->Height = desc.Height;
	(in)->PixelFormat = desc.PixelFormat;
	(in)->Stride = sizeof(DirectX::PackedVector::XMCOLOR);
	(in)->Scan0 = data.get();
}
