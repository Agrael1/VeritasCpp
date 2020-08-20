#include "IndexBuffer.h"

IndexBuffer::IndexBuffer(VeritasEngine& vin,const void* vertices_in)
{
	VBUFFER_DESC bufdesc;
	bufdesc.BindFlags = VBIND_FLAG::INDEX_BUFFER;
	bufdesc.StructureByteStride = sizeof(VertexIndex);
	bufdesc.ByteWidth = sizeof(bunnyIndices);

	GetDevice(vin)->CreateBuffer(&bufdesc, &pVBuf, vertices_in);
}

void IndexBuffer::Bind(VeritasEngine& vin)
{
	GetContext(vin)->IASetIndexBuffer(pVBuf.Get(), VFORMAT::FORMAT_UINT32, 0u);
}
