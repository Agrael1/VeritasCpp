#pragma once
#include <Engine\Arch\IndexBuffer.h>
#include <memory>
#include <vector>
#include "Engine\Arch\VertexBuffer.h"
#include "Engine\Arch\VertexShader.h"
#include "Engine\Arch\PixelShader.h"
#include "Engine\Arch\InputLayout.h"
#include "Engine\Arch\ConstantBuffer.h"
#include "Engine\Arch\Drawable.h"


class XModel : public Drawable
{
public:
	XModel(VeritasEngine& vin)
	{
		pIndices = std::make_unique<IndexBuffer>(vin, bunnyIndices);
		pVertices = std::make_unique<VertexBuffer>(vin, bunnyVertices);

		binds.emplace_back(std::make_unique<VertexShader>("SimpleVS"));
		binds.emplace_back(std::make_unique<PixelShader>("SimplePS"));

		VINPUT_ELEMENT x[2] = {
			{VFORMAT::FORMAT_R32G32B32_FLOAT, 0, 0},
			{VFORMAT::FORMAT_R32G32B32_FLOAT, 0, offsetof(BunnyVertex, normal)}
		};

		binds.emplace_back(std::make_unique<InputLayout>(vin, x));
		binds.emplace_back(std::make_unique<TransformCbuf>(vin, *this));
	}
public:
	DirectX::XMMATRIX GetTransformXM() const noexcept
	{
		return DirectX::XMMatrixTranslation(0,0,0);
	}
};