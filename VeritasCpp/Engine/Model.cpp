#include "Model.h"

XModel::XModel(VeritasEngine& vin)
{
	constexpr auto x = Plane::Indices();
	auto y = Plane::Vertices();
	pIndices = std::make_unique<IndexBuffer>(vin, x.data(), sizeof(uint32_t), x.size());
	pVertices = std::make_unique<VertexBuffer>(vin, y.data(), sizeof(Plane::PVtx), y.size());

	binds.emplace_back(std::make_unique<VertexShader>("PhongVS"));
	binds.emplace_back(std::make_unique<PixelShader>("PhongPS"));

	VINPUT_ELEMENT z[2] = {
		{ VFORMAT::FORMAT_R32G32B32_FLOAT, 0, 0 },
		{ VFORMAT::FORMAT_R32G32B32_FLOAT, 0, offsetof(Plane::PVtx, normal) }
	};

	binds.emplace_back(std::make_unique<InputLayout>(vin, z));
	binds.emplace_back(std::make_unique<TransformCbuf>(vin, *this));
}

DirectX::XMMATRIX XModel::GetTransformXM() const noexcept
{
	return DirectX::XMMatrixIdentity();
}
