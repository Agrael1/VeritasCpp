#include "VertexBuffer.h"

VertexBuffer::VertexBuffer(VeritasEngine& vin, const void* vertices_in)
{
	VBUFFER_DESC bufdesc;
	bufdesc.BindFlags = VBIND_FLAG::VERTEX_BUFFER;
	bufdesc.StructureByteStride = stride = sizeof(BunnyVertex);
	bufdesc.ByteWidth = sizeof(bunnyVertices);

	GetDevice(vin)->CreateBuffer(&bufdesc, &pVBuf, vertices_in);
}
VertexBuffer::VertexBuffer(VeritasEngine& vin, const void* vertices_in, size_t VertSz, size_t VertCnt)
{
	VBUFFER_DESC bufdesc;
	bufdesc.BindFlags = VBIND_FLAG::INDEX_BUFFER;
	bufdesc.StructureByteStride = stride = VertSz;
	bufdesc.ByteWidth = VertCnt * VertSz;

	GetDevice(vin)->CreateBuffer(&bufdesc, &pVBuf, vertices_in);
}

void VertexBuffer::Bind(VeritasEngine& vin)
{
	const uint32_t offset = 0;
	GetContext(vin)->IASetVertexBuffers(0, 1, pVBuf.GetAddressOf(), &stride, &offset);
}
