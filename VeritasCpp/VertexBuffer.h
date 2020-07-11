#pragma once
#include <Engine\Arch\Bindable.h>
#include <Bunny.h>


class VertexBuffer : public Bindable
{
public:
	VertexBuffer(VeritasEngine& vin, void* vertices_in)
	{
		VBUFFER_DESC bufdesc;
		bufdesc.BindFlags = VBIND_FLAG::VERTEX_BUFFER;
		bufdesc.StructureByteStride = stride = sizeof(BunnyVertex);
		bufdesc.ByteWidth = sizeof(bunnyVertices);

		GetDevice(vin).CreateBuffer(&bufdesc, &pVBuf, vertices_in);
	}
public:
	void Bind(VeritasEngine& vin)override
	{
		const uint32_t offset = 0;
		GetContext(vin).IASetVertexBuffers(0, 1, pVBuf.GetAddressOf(), &stride, &offset);
	}
private:
	uint32_t stride;
	wrl::ComPtr<IVBuffer> pVBuf;
};

