#define DEVICE_IMPL
#include "Buffer.h"
#include "Texture.h"
#include "Device.h"



HRESULT __stdcall CreateBuffer(VGraphicsDevice* This, const VBUFFER_DESC* desc, IVBuffer** _out_Bufptr, const void* initialData)
{
	MakeAndInitialize(_out_Bufptr, VBuffer, desc, initialData);
	return S_OK;
}
HRESULT __stdcall CreateTexture2D(VGraphicsDevice* This, const VTEXTURE_DESC* desc, IVTexture** _out_texptr, const void* initialData)
{
	MakeAndInitialize(_out_texptr, VTexture, desc, initialData);
	return S_OK;
}
HRESULT __stdcall CreateRenderTargetView(VGraphicsDevice* This, IVTexture* resource, VRTV_DESC* _out_rtv)
{
	return S_OK;
}
HRESULT __stdcall CreateDepthStencilView(VGraphicsDevice* This, IVTexture* resource, VDSV_DESC* _out_rtv)
{
	return S_OK;
}

HRESULT RuntimeClassInitialize()
{
	return S_OK;
}
void RuntimeClassDestroy(VGraphicsDevice* This)
{

}
VirtualTable(IVDevice)
{
	._unknwn = __COM_IUnknown,
	.CreateBuffer = CreateBuffer,
	.CreateTexture2D = CreateTexture2D,
	.CreateRenderTargetView = CreateRenderTargetView,
	.CreateDepthStencilView = CreateDepthStencilView,
};

ENDCLASSDESC