#pragma once
#include "SwapChain.h"
#include <array>

constexpr auto MaxRenderTargets = 2u;
constexpr auto MaxViewPorts = 2u;

struct ViewPort
{
	uint32_t Width;
	uint32_t Height;
	uint32_t TopLeftX;
	uint32_t TopLeftY;
};

class VContext
{
public:
	VContext();
public:
	HRESULT RSSetViewports(uint32_t numVPs, const ViewPort* _arr_VPs)
	{
		if (numVPs > MaxRenderTargets) return E_INVALIDARG;
		std::copy_n(_arr_VPs, numVPs, RSViewPorts.begin());
		return S_OK;
	}
	HRESULT OMSetRenderTargets(uint32_t numViews, const RenderTargetView* const _arr_RTVs)noxnd
	{
		if (numViews > MaxRenderTargets) return E_INVALIDARG;
		std::copy_n(_arr_RTVs, numViews, OMRenderTargets.begin());
		return S_OK;
	}
	void ClearRenderTarget(RenderTargetView* rtv, DirectX::PackedVector::XMCOLOR col)
	{
		std::fill((DirectX::PackedVector::XMCOLOR*)rtv->Scan0, (DirectX::PackedVector::XMCOLOR*)rtv->Scan0 + size_t(rtv->Width) * rtv->Height, col);
	}
	void Draw();
private:
	std::array<RenderTargetView, MaxRenderTargets> OMRenderTargets;
	std::array<ViewPort, MaxViewPorts> RSViewPorts;
};