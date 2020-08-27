#include "IndexBuffer.h"

IndexBuffer::IndexBuffer(VeritasEngine& vin, const void* vertices_in)
{
	VBUFFER_DESC bufdesc;
	bufdesc.BindFlags = VBIND_FLAG::INDEX_BUFFER;
	bufdesc.StructureByteStride = sizeof(VertexIndex);
	bufdesc.ByteWidth = sizeof(bunnyIndices);
	indexCount = sizeof(bunnyIndices) / sizeof(VertexIndex);

	GetDevice(vin)->CreateBuffer(&bufdesc, &pVBuf, vertices_in);
}

IndexBuffer::IndexBuffer(VeritasEngine& vin, const void* vertices_in, size_t VertSz, size_t VertCnt)
{
	VBUFFER_DESC bufdesc;
	bufdesc.BindFlags = VBIND_FLAG::INDEX_BUFFER;
	bufdesc.StructureByteStride = VertSz;
	bufdesc.ByteWidth = VertCnt * VertSz;
	indexCount = VertCnt;

	GetDevice(vin)->CreateBuffer(&bufdesc, &pVBuf, vertices_in);
}

void IndexBuffer::Bind(VeritasEngine& vin)
{
	GetContext(vin)->IASetIndexBuffer(pVBuf.Get(), VFORMAT::FORMAT_UINT32, 0u);
}

uint32_t IndexBuffer::GetCount() const noexcept
{
	return indexCount;
}
