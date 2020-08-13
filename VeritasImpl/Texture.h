#pragma once
#include <Interfaces.h>
#include <memory>
#include <DirectXMath\Inc\DirectXPackedVector.h>

class VTexture : public wrl::RuntimeClass<wrl::RuntimeClassFlags<wrl::ClassicCom>, IVTexture>
{
	friend class VSwapChain;
	friend class VGraphicsDevice;
public:
	HRESULT RuntimeClassInitialize(const VTEXTURE_DESC* in_desc, const void* _in_data = nullptr);
public:
	VTexture() = default;
public:
	virtual void GetDesc(VTEXTURE_DESC* _out_desc)const override;
private:
	void GetView(VRTV_DESC* in)noexcept;
private:
	VTEXTURE_DESC desc{ 0 };
	std::unique_ptr<uint8_t[]> data{ 0 };
};

