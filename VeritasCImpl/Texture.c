#define TEXTURE_IMPL
#include "Texture.h"

static void __stdcall GetDesc(const VTexture* This, VTEXTURE_DESC* _out_desc)
{
	*_out_desc = This->desc;
}
void GetView(VTexture* This, VRTV_DESC* _out_rtv)
{
	(_out_rtv)->Width = This->desc.Width;
	(_out_rtv)->Height = This->desc.Height;
	(_out_rtv)->PixelFormat = This->desc.PixelFormat;
	(_out_rtv)->Stride = This->size * This->desc.Width;
	(_out_rtv)->Scan0 = This->data;
}

HRESULT RuntimeClassInitialize(VTexture* This, const VTEXTURE_DESC* in_desc, const void* _in_data)
{
	This->desc = *in_desc;
	if (!This->desc.Height || !This->desc.Width) return E_INVALIDARG;

	if (This->desc.PixelFormat & (32ull << 8))
	{
		This->size = 4;
	}
	else if (This->desc.PixelFormat & (64ull << 8))
	{
		This->size = 8;
	}
	This->data = malloc(in_desc->Width * in_desc->Height * This->size);


	if (_in_data)
	{
		size_t dsize = This->desc.Width * This->desc.Height * This->size;
		memcpy_s(This->data, dsize, (const uint8_t*)_in_data, dsize);
	}

	return S_OK;
}
void RuntimeClassDestroy(VTexture* This)
{
	if (This->data) free(This->data);
}
VirtualTable(IVTexture)
{
	._unknwn = __COM_IUnknown,
	.GetDesc = GetDesc
};

ENDCLASSDESC
