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
		float3 viewPos; // Position;
		float3 viewNormal; // Normal;
		float4 SV_Position; // SV_Position;
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
	void UpdateConstants(uint8_t*const* constants)override
	{
		std::copy(constants[0], constants[0] + sizeof(CBuffer), (uint8_t*)&cbuf);
	};
};

