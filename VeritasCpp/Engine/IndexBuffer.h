#pragma once
#include "Bindable.h"

class IndexBuffer : public Bindable
{
public:
	IndexBuffer(VeritasEngine& vin, const void* indices, size_t TypeSize, size_t Count)noexcept;
public:
	void Bind(VeritasEngine& vin) override;
	uint32_t GetCount()const noexcept;
private:
	wrl::ComPtr<IVBuffer> pVBuf;
	uint32_t indexCount = 0;
	VFORMAT format = VFORMAT::FORMAT_NONE;
};