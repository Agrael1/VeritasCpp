#include "VertexBuffer.h"

VertexBuffer::VertexBuffer(VeritasEngine& vin, void* vertices_in)
{
	VBUFFER_DESC bufdesc;
	bufdesc.BindFlags = VBIND_FLAG::VERTEX_BUFFER;
	bufdesc.StructureByteStride = stride = sizeof(BunnyVertex);
	bufdesc.ByteWidth = sizeof(bunnyVertices);

	GetDevice(vin)->CreateBuffer(&bufdesc, &pVBuf, vertices_in);
}

void VertexBuffer::Bind(VeritasEngine& vin)
{
	const uint32_t offset = 0;
	GetContext(vin)->IASetVertexBuffers(0, 1, pVBuf.GetAddressOf(), &stride, &offset);
}
