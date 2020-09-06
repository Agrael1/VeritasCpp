#pragma once
#include "Window.h"
#include <Interfaces.h>
#include <Inc\DirectXPackedVector.h>

using RenderTargetView = VRTV_DESC;
using DepthStencilView = VDSV_DESC;

class VeritasEngine
{
	friend class Bindable;
public:
	VeritasEngine(uint16_t width, uint16_t height);
public:
	Window& Wnd();
	void BeginFrame(float r, float g, float b)noexcept;
	void EndFrame();
	DirectX::XMMATRIX GetCamera()const noexcept;
	void SetCamera(DirectX::XMMATRIX Camera)noexcept;
	void DrawIndexed(uint32_t count)noexcept;
	DirectX::XMMATRIX GetProjection() const noexcept;
	void SetProjection(DirectX::FXMMATRIX proj) noexcept;
	uint16_t GetWidth() const noexcept;
	uint16_t GetHeight() const noexcept;
private:
	DirectX::XMMATRIX projection;
	DirectX::XMMATRIX camera;

	Window window;
	wrl::ComPtr<IVDevice> pDevice;
	wrl::ComPtr<IVContext> pContext;
	wrl::ComPtr<IVSwapChain> pSwap;
	wrl::ComPtr<IVTexture> pDepthStencil;
public:
	RenderTargetView rtv{ 0 };
	DepthStencilView dsv{ 0 };
};