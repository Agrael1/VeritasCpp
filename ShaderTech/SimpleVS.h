#pragma once
#include "Shader.h"

class SimpleVS : public VertexShader<SimpleVS>
{
public:
	struct CBuffer
	{
		matrix modelView;
		matrix modelViewProj;
	}cbuf;

	struct VSOut
	{
		float3A viewPos; // Position;
		float3A viewNormal; // Normal;
		float4A SV_Position; // SV_Position;
	};
public:
	VSOut main(float3 pos, float3 Normal)
	{
		VSOut vso;
		vso.viewPos = dx::XMVector3Transform(pos, cbuf.modelView);
		vso.viewNormal = dx::XMVector3TransformNormal(Normal, cbuf.modelView);
		vso.SV_Position = dx::XMVector3Transform(pos, cbuf.modelViewProj);
		return vso;
	}
public:
	void UpdateConstants(void*const* constants)override
	{
		cbuf = constants[0] ? *static_cast<const CBuffer*>(constants[0]) : CBuffer{};
	};
};

