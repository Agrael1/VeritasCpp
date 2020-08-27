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

	VFCreateDevice(window.GetWindowHandle(), &pGfx, &pContext);
	VFCreateSwapChain(&sd, pGfx.Get(), &pSwap);
	pSwap->GetRenderTarget(0, &rtv);
	pContext->OMSetRenderTargets(1, &rtv);
}

Window& VeritasEngine::Wnd()
{
	return window;
}



void VeritasEngine::BeginFrame(float r, float g, float b) noexcept
{
	DirectX::PackedVector::XMCOLOR col(r,g,b,1.f);
	pContext->ClearRenderTarget(&rtv, col);
	//pContext->ClearDepthStencil(pDSV.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0u);
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

