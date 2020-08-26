#pragma once
#include <Engine\Arch\IndexBuffer.h>
#include <memory>
#include <vector>
#include "Engine\Arch\VertexBuffer.h"
#include "Engine\Arch\VertexShader.h"
#include "Engine\Arch\PixelShader.h"
#include "Engine\Arch\InputLayout.h"


class XModel : public Bindable
{
public:
	XModel(VeritasEngine& vin)
	{
		bindables.emplace_back(std::make_unique<IndexBuffer>(vin, bunnyIndices));
		bindables.emplace_back(std::make_unique<VertexBuffer>(vin, bunnyVertices));
		bindables.emplace_back(std::make_unique<VertexShader>("SimpleVS"));
		bindables.emplace_back(std::make_unique<PixelShader>("SimplePS"));

		VINPUT_ELEMENT x[2] = {
			{VFORMAT::FORMAT_R32G32B32_FLOAT, 0, 0},
			{VFORMAT::FORMAT_R32G32B32_FLOAT, 0, offsetof(BunnyVertex, normal)}
		};

		bindables.emplace_back(std::make_unique<InputLayout>(vin, x));
	}
public:
	virtual void Bind(class VeritasEngine& in) {};
	void Draw(VeritasEngine& vin)
	{
		GetContext(vin)->IASetPrimitiveTopology(VPRIMITIVE_TOPOLOGY::TRIANGLELIST);
		for (auto& b : bindables)
		{
			b->Bind(vin);
		}
		GetContext(vin)->DrawIndexed(2092 * 3);
	}
private:
	std::vector<std::unique_ptr<Bindable>> bindables;
};