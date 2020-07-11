#pragma once
#include <Engine\Arch\IndexBuffer.h>
#include "VertexBuffer.h"


class XModel : public Bindable
{
public:
	XModel(VeritasEngine& vin)
	{
		bindables.emplace_back(std::make_unique<IndexBuffer>(vin, bunnyIndices));
		bindables.emplace_back(std::make_unique<VertexBuffer>(vin, bunnyVertices));
	}
public:
	void Draw(VeritasEngine& vin)
	{
		for (auto& b : bindables)
		{
			b->Bind(vin);
		}
		GetContext(vin).Draw();
	}
private:
	std::vector<std::unique_ptr<Bindable>> bindables;
};