#include "InputLayout.h"

InputLayout::InputLayout(VeritasEngine& gfx,
	const std::vector<VINPUT_ELEMENT>& layout)
{
	//INFOMAN(gfx);

	//GFX_THROW_INFO(GetDevice(gfx).CreateInputLayout(
	//	layout.data(), (UINT)layout.size(),
	//	pVertexShaderBytecode->GetBufferPointer(),
	//	pVertexShaderBytecode->GetBufferSize(),
	//	&pInputLayout
	//));
}

void InputLayout::Bind(VeritasEngine& gfx) noexcept
{
	/*GetContext(gfx).IASetInputLayout(pInputLayout.Get());*/
}