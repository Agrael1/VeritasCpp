#pragma once
#include <Engine\Arch\Bindable.h>
#include <Bunny.h>


class IndexBuffer : public Bindable
{
public:
	IndexBuffer(VeritasEngine& vin,const void* vertices_in);
public:
	void Bind(VeritasEngine& vin) override;
private:
	wrl::ComPtr<IVBuffer> pVBuf;
};