#include "VeritasEngine.h" 


VeritasEngine::VeritasEngine(uint16_t width, uint16_t height)
	:window(width,height,L"VTest")
{
	VSWAP_CHAIN_DESC sd;
	sd.BufferCount = 1;
	sd.Height = height;
	sd.Width = width;
	sd.OutputWindow = 0;
	sd.Windowed = TRUE;	

	
	VFCreateDevice(window.GetWindowHandle(), &pDevice, &pContext);
	VFCreateSwapChain(&sd, pDevice.Get(), &pSwap);
	pSwap->GetRenderTarget(0, &rtv);
	pContext->OMSetRenderTargets(1, &rtv);


	VVIEWPORT_DESC vp;
	vp.Height = height;
	vp.Width = width;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;

	pContext->RSSetViewport(1, &vp);

	VTEXTURE_DESC ds;
	ds.BindFlags = VBIND_FLAG::DEPTH_STENCIL;
	ds.Height = height;
	ds.Width = width;
	ds.PixelFormat = VPIXEL_FORMAT::FLOAT32bpp;
	pDevice->CreateTexture2D(&ds, &pDepthStencil);

	pDevice->CreateDepthStencilView(pDepthStencil.Get(), &dsv);
	pContext->OMSetDepthStencil(&dsv);
}

Window& VeritasEngine::Wnd()
{
	return window;
}



void VeritasEngine::BeginFrame(float r, float g, float b) noexcept
{
	DirectX::PackedVector::XMCOLOR col(r,g,b,1.f);
	pContext->ClearRenderTarget(&rtv, col);
	pContext->ClearDepthStencil(&dsv, INFINITY);
}

void VeritasEngine::EndFrame()
{
	pSwap->Present();
}

DirectX::XMMATRIX VeritasEngine::GetCamera() const noexcept
{
	return camera;
}
void VeritasEngine::SetCamera(DirectX::XMMATRIX Camera)noexcept
{
	camera = Camera;
}
void VeritasEngine::SetProjection(DirectX::FXMMATRIX proj) noexcept
{
	projection = proj;
}
DirectX::XMMATRIX VeritasEngine::GetProjection() const noexcept
{
	return projection;
}
void VeritasEngine::DrawIndexed(uint32_t count) noexcept
{
	pContext->DrawIndexed(count);
}
uint16_t VeritasEngine::GetWidth() const noexcept
{
	return (uint16_t)rtv.Width;
}
uint16_t VeritasEngine::GetHeight() const noexcept
{
	return (uint16_t)rtv.Height;
}

