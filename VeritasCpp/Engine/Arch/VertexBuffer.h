#pragma once
#include <Engine\Arch\Bindable.h>
#include <Bunny.h>


class VertexBuffer : public Bindable
{
public:
	VertexBuffer(VeritasEngine& vin, void* vertices_in);
public:
	void Bind(VeritasEngine& vin)override;
private:
	uint32_t stride;
	wrl::ComPtr<IVBuffer> pVBuf;
};

