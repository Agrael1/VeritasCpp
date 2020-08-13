#include "DepthStencil.h"

DepthStencil::DepthStencil(VeritasEngine& in, uint32_t width, uint32_t height)
{
	VTEXTURE_DESC vtex;
	vtex.BindFlags = VBIND_FLAG::DEPTH_STENCIL;
	vtex.Width = width;
	vtex.Height = height;
	vtex.PixelFormat = VPIXEL_FORMAT::ARGB32bpp;
	GetDevice(in)->CreateTexture2D(&vtex, &pDS, nullptr);
	GetDevice(in)->CreateDepthStencilView(pDS.Get(), &DSV);
}

void DepthStencil::Bind(VeritasEngine& in)
{
	GetContext(in)->OMSetDepthStencil(&DSV);
}
