#pragma once
#include "Bindable.h"

template<typename C>
class ConstantBuffer : public Bindable
{
public:
	ConstantBuffer(VeritasEngine& gfx, const C& consts, UINT slot = 0u)
		:slot(slot)
	{
		VBUFFER_DESC cbd = {};
		cbd.BindFlags = VBIND_FLAG::CONSTANT_BUFFER;
		cbd.ByteWidth = sizeof(consts);
		cbd.StructureByteStride = 0u;

		GetDevice(gfx)->CreateBuffer(&cbd, &pConstantBuffer, &consts);
	}
	ConstantBuffer(VeritasEngine& gfx, UINT slot = 0u)
		:slot(slot)
	{
		VBUFFER_DESC cbd = {};
		cbd.BindFlags = VBIND_FLAG::CONSTANT_BUFFER;
		cbd.ByteWidth = sizeof(C);
		cbd.StructureByteStride = 0u;

		GetDevice(gfx)->CreateBuffer(&cbd, &pConstantBuffer);
	}
public:
	void Update(VeritasEngine& gfx, const C& consts)
	{
		VMAPPED_SUBRESOURCE msr = {};
		GetContext(gfx)->Map(pConstantBuffer.Get(), &msr);
		memcpy_s(msr.pData, msr.DepthPitch, &consts, sizeof(consts));
		GetContext(gfx)->Unmap(pConstantBuffer.Get());
	}
protected:
	wrl::ComPtr<IVBuffer> pConstantBuffer;
	UINT slot;
};

template<typename C>
class VertexConstantBuffer : public ConstantBuffer<C>
{
	using ConstantBuffer<C>::pConstantBuffer;
	using ConstantBuffer<C>::slot;
	using Bindable::GetContext;
public:
	using ConstantBuffer<C>::ConstantBuffer;
	void Bind(VeritasEngine& gfx)override
	{
		GetContext(gfx)->VSSetConstantBuffers(slot, 1u, pConstantBuffer.GetAddressOf());
	}
};

template<typename C>
class PixelConstantBuffer : public ConstantBuffer<C>
{
	using ConstantBuffer<C>::pConstantBuffer;
	using ConstantBuffer<C>::slot;
	using Bindable::GetContext;
public:
	using ConstantBuffer<C>::ConstantBuffer;
	void Bind(VeritasEngine& gfx) override
	{
		GetContext(gfx)->PSSetConstantBuffers(slot, 1u, pConstantBuffer.GetAddressOf());
	}
};

#include <Inc\DirectXMath.h>
#include <memory>

class TransformCbuf : public Bindable
{
protected:
	struct Transforms
	{
		DirectX::XMMATRIX modelView;
		DirectX::XMMATRIX modelViewProj;
	};
public:
	TransformCbuf(VeritasEngine& gfx, const class Drawable& parent, UINT slot = 0u);
	void Bind(VeritasEngine& gfx) override;
protected:
	void UpdateBindImpl(VeritasEngine& gfx, const Transforms& tf) noexcept;
	Transforms GetTransforms(VeritasEngine& gfx) noexcept;
private:
	static std::unique_ptr<VertexConstantBuffer<Transforms>> pVcbuf;
	const Drawable* pParent = nullptr;
};