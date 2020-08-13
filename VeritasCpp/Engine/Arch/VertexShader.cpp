#include "VertexShader.h"

VertexShader::VertexShader(std::string_view shadertag)
{
	VFMakeShader(shadertag.data(), &vs);
}

void VertexShader::Bind(VeritasEngine& in)
{
	GetContext(in)->VSSetShader(vs.Get());
}
