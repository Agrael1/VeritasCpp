#include "IndexBuffer.h"

IndexBuffer::IndexBuffer(VeritasEngine& vin, const void* indices, size_t TypeSize, size_t Count) noexcept
{
	VBUFFER_DESC bufdesc;
	bufdesc.BindFlags = VBIND_FLAG::INDEX_BUFFER;
	bufdesc.StructureByteStride = TypeSize;
	bufdesc.ByteWidth = TypeSize*Count;
	indexCount = Count;
	format = VFORMAT(TypeSize);

	GetDevice(vin)->CreateBuffer(&bufdesc, &pVBuf, indices);
}

void IndexBuffer::Bind(VeritasEngine& vin)
{
	GetContext(vin)->IASetIndexBuffer(pVBuf.Get(), format, 0u);
}

uint32_t IndexBuffer::GetCount() const noexcept
{
	return indexCount;
}
