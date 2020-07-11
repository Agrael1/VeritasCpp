#pragma once
#include <Engine\Arch\Bindable.h>
#include <Bunny.h>


class IndexBuffer : public Bindable
{
public:
	IndexBuffer(VeritasEngine& vin, void* vertices_in)
	{
		VBUFFER_DESC bufdesc;
		bufdesc.BindFlags = VBIND_FLAG::INDEX_BUFFER;
		bufdesc.StructureByteStride = sizeof(VertexIndex);
		bufdesc.ByteWidth = sizeof(bunnyIndices);

		GetDevice(vin).CreateBuffer(&bufdesc, &pVBuf, vertices_in);
	}
public:
	void Bind(VeritasEngine& vin) override
	{
		GetContext(vin).IASetIndexBuffer(pVBuf.Get(), VFORMAT::FORMAT_UINT32, 0u);
	}
private:
	wrl::ComPtr<IVBuffer> pVBuf;
};