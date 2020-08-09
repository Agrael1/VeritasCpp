#pragma once
#include <Interfaces.h>
#include <array>
#include <span>
#include <algorithm>

#include "Buffer.h"
#include "InputLayout.h"
#include "Texture.h"

#include <DirectXMath\Inc\DirectXPackedVector.h>


class VContext: public wrl::RuntimeClass<wrl::RuntimeClassFlags<wrl::ClassicCom>, IVContext>
{
public:
	VContext() = default;
public:
	HRESULT __stdcall IASetPrimitiveTopology(VPRIMITIVE_TOPOLOGY Topology)override;
	HRESULT __stdcall IASetVertexBuffers(uint32_t StartSlot, uint32_t NumBuffers, IVBuffer* const* ppVertexBuffers, const uint32_t* pStrides, const uint32_t* pOffsets)override;
	HRESULT __stdcall IASetIndexBuffer(IVBuffer* indexBuffer, VFORMAT format, uint32_t offsetBytes)override;
	HRESULT __stdcall IASetInputLayout(IVInputLayout* pInputLayout)override;
	HRESULT __stdcall VSSetShader(IVShader* pVertexShader)override;
	HRESULT __stdcall VSSetConstantBuffers(uint32_t StartSlot, uint32_t NumBuffers, IVBuffer* const* ppConstantBuffers)override;
	HRESULT __stdcall RSSetViewports(uint32_t numVPs, const VVIEWPORT_DESC* _arr_VPs)override;
	HRESULT __stdcall OMSetRenderTargets(uint32_t numViews, const VRTV_DESC* const _arr_RTVs)override;
	HRESULT __stdcall ClearRenderTarget(VRTV_DESC* rtv, uint32_t col)override;
private:
	std::array<VRTV_DESC, MaxRenderTargets> OMRenderTargets;
	std::array<VVIEWPORT_DESC, MaxViewPorts> RSViewPorts;
	std::span<uint8_t> IAIndexBuffer;
	VFORMAT IAIndexFormat;
	VPRIMITIVE_TOPOLOGY IATopology;
	VInputLayout* IAInputLayout;
	std::array<std::span<uint8_t>, 4> IAVertexBuffers;
	std::array<uint32_t, 4> IAVBStrides;
	wrl::ComPtr<IVShader> VSVertexShader;
	std::vector<wrl::ComPtr<IVBuffer>> VSConstantBuffers;
};