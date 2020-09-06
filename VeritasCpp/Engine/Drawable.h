#pragma once
#include <vector>
#include <memory>
#include <Inc\DirectXMath.h>
#include "IndexBuffer.h"
#include "VertexBuffer.h"


class Drawable
{
public:
	Drawable() = default;
	Drawable(const Drawable&) = delete;
	virtual ~Drawable();
public:
	virtual DirectX::XMMATRIX GetTransformXM() const noexcept = 0;
	uint32_t GetIndexCount() const noexcept;
	void Draw(VeritasEngine& gfx) const;
protected:
	std::shared_ptr<IndexBuffer> pIndices;
	std::shared_ptr<VertexBuffer> pVertices;
	std::vector<std::unique_ptr<Bindable>> binds;
};