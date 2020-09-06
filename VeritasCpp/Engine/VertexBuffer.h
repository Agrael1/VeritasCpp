#pragma once
#include "Bindable.h"
#include <span>

class VertexBuffer : public Bindable
{
public:
	VertexBuffer(VeritasEngine& vin, const void* vertices, size_t VertexSize, size_t Count);
public:
	void Bind(VeritasEngine& vin)override;
private:
	uint32_t stride;
	wrl::ComPtr<IVBuffer> pVBuf;
};
