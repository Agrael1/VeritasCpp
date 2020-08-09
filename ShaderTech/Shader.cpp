#include "Shader.h"

void __stdcall VertexShaderBase::UpdateConstants(uint8_t* const* constants) {}

void __stdcall VertexShaderBase::Invoke(const void* vs_in, void* _out_vertex)
{
	Invoke(*static_cast<const DumbVertex*>(vs_in), *static_cast<DumbVSOut*>(_out_vertex));
}

void __stdcall VertexShaderBase::GetByteCode(const char** _out_bytecode)
{
	*_out_bytecode = ByteCode.c_str();
}
