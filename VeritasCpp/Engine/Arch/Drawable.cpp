#include "../VeritasEngine.h"
#include "Drawable.h"
#include "IndexBuffer.h"
#include "VertexBuffer.h"


UINT Drawable::GetIndexCount() const noexcept
{
	return pIndices->GetCount();
}

void Drawable::Draw(VeritasEngine& gfx) const
{
	for (auto& b : binds)
	{
		b->Bind(gfx);
	}
	pIndices->Bind(gfx);
	pVertices->Bind(gfx);
	gfx.DrawIndexed(pIndices->GetCount());
}

Drawable::~Drawable()
{}
