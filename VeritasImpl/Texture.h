#pragma once
#include <Interfaces.h>
#include <memory>

class VTexture : public wrl::RuntimeClass<wrl::RuntimeClassFlags<wrl::ClassicCom>, IVTexture>
{
	friend class VSwapChain;
	friend class VGraphicsDevice;
public:
	HRESULT RuntimeClassInitialize(const VTEXTURE_DESC* in_desc, const void* _in_data = nullptr);
public:
	VTexture() = default;
public:
	void __stdcall GetDesc(VTEXTURE_DESC* _out_desc)const override;
private:
	void GetView(VRTV_DESC* in)noexcept;
private:
	VTEXTURE_DESC desc{ 0 };
	uint32_t size;
	std::unique_ptr<uint8_t[]> data;
};