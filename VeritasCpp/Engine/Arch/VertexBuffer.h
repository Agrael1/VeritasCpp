#pragma once
#include <Engine\Arch\Bindable.h>
#include <Bunny.h>


class VertexBuffer : public Bindable
{
public:
	VertexBuffer(VeritasEngine& vin,const void* vertices_in);
	VertexBuffer(VeritasEngine& vin, const void* vertices_in, size_t VertSz, size_t VertCnt);
public:
	void Bind(VeritasEngine& vin)override;
private:
	uint32_t stride;
	wrl::ComPtr<IVBuffer> pVBuf;
};

