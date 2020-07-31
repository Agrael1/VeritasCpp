#pragma once
#include <Engine\Arch\IndexBuffer.h>
#include "VertexBuffer.h"
#include "VertexShader.h"


class XModel : public Bindable
{
public:
	XModel(VeritasEngine& vin)
	{
		//bindables.emplace_back(std::make_unique<IndexBuffer>(vin, bunnyIndices));
		//bindables.emplace_back(std::make_unique<VertexBuffer>(vin, bunnyVertices));
		SimpleVS v;
	}
public:
	virtual void Bind(class VeritasEngine& in) {};
	void Draw(VeritasEngine& vin)noxnd
	{
		GetContext(vin).IASetPrimitiveTopology(VPRIMITIVE_TOPOLOGY::TRIANGLELIST);
		for (auto& b : bindables)
		{
			b->Bind(vin);
		}
	}
private:
	std::vector<std::unique_ptr<Bindable>> bindables;
};