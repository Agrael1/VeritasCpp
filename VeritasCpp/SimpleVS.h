#pragma once
#include <VertexShader.h>

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
		float2 tc; // Texcoord;
		float4 SV_Position; // SV_Position;
	};
public:
	VSOut main(float3 pos, float3 Normal, float2 Texcoord)
	{
		VSOut vso;
		vso.viewPos = dx::XMVector3Transform(pos, cbuf.modelView);
		vso.viewNormal = dx::XMVector3TransformNormal(Normal, cbuf.modelView);
		vso.SV_Position = dx::XMVector3Transform(pos, cbuf.modelViewProj);
		vso.tc = Texcoord;
		return vso;
	}
public:
	using RQVSOutT = VSOut;
	void UpdateConstants(std::span<uint8_t> constants)override
	{
		std::copy(constants.begin(), constants.end(), (uint8_t*)&cbuf);
	};
};

#ifndef VS_IMPL
#undef float
#endif

