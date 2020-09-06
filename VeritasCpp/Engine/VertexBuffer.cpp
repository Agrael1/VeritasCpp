#include "VertexBuffer.h"

VertexBuffer::VertexBuffer(VeritasEngine& vin, const void* vertices, size_t VertexSize, size_t Count)
{
	VBUFFER_DESC bufdesc;
	bufdesc.BindFlags = VBIND_FLAG::INDEX_BUFFER;
	bufdesc.StructureByteStride = stride = (uint32_t)VertexSize;
	bufdesc.ByteWidth = uint32_t(VertexSize * Count);

	GetDevice(vin)->CreateBuffer(&bufdesc, &pVBuf, vertices);
}

void VertexBuffer::Bind(VeritasEngine& vin)
{
	const uint32_t offset = 0;
	GetContext(vin)->IASetVertexBuffers(0, 1, pVBuf.GetAddressOf(), &stride, &offset);
}
