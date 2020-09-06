#include "PixelShader.h"

PixelShader::PixelShader(std::string_view shaderName)
{
	VFMakeShader(shaderName.data(), &pPS);
}

void PixelShader::Bind(VeritasEngine& vin)
{
	GetContext(vin)->PSSetShader(pPS.Get());
}
