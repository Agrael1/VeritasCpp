#include "../VeritasEngine.h"
#include "Drawable.h"
#include "ConstantBuffer.h"


std::unique_ptr<VertexConstantBuffer<TransformCbuf::Transforms>> TransformCbuf::pVcbuf;

TransformCbuf::TransformCbuf(VeritasEngine& gfx, const Drawable& parent, UINT slot)
{
	pParent = &parent;

	if (!pVcbuf)
	{
		pVcbuf = std::make_unique<VertexConstantBuffer<Transforms>>(gfx, slot);
	}
}

void TransformCbuf::Bind(VeritasEngine& gfx)
{
	UpdateBindImpl(gfx, GetTransforms(gfx));
}

void TransformCbuf::UpdateBindImpl(VeritasEngine& gfx, const Transforms& tf) noexcept
{
	assert(pParent != nullptr);
	pVcbuf->Update(gfx, tf);
	pVcbuf->Bind(gfx);
}
TransformCbuf::Transforms TransformCbuf::GetTransforms(VeritasEngine& gfx) noexcept
{
	assert(pParent != nullptr);
	const auto modelView = pParent->GetTransformXM() * gfx.GetCamera();
	return {
		DirectX::XMMatrixTranspose(modelView),
		DirectX::XMMatrixTranspose(
			modelView *
			gfx.GetProjection()
		)
	};
}
