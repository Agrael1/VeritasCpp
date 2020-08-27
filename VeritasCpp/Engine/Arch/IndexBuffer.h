#pragma once
#include <Engine\Arch\Bindable.h>
#include <Bunny.h>


class IndexBuffer : public Bindable
{
public:
	IndexBuffer(VeritasEngine& vin, const void* vertices_in);
	IndexBuffer(VeritasEngine& vin, const void* vertices_in, size_t VertSz, size_t vertsCnt);
public:
	void Bind(VeritasEngine& vin) override;
	uint32_t GetCount()const noexcept;
private:
	wrl::ComPtr<IVBuffer> pVBuf;
	uint32_t indexCount = 0;
};