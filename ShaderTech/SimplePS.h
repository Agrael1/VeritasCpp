#pragma once
#include "Shader.h"

struct LightVectorData
{
	float3U vToL;
	float3U dirToL;
	float1U distToL;
};

inline LightVectorData CalculateLightVectorData(const float3U lightPos, const float3 fragPos)
{
	LightVectorData lv;
	lv.vToL = lightPos - fragPos;
	lv.distToL = dx::XMVector3Length(lv.vToL);
	lv.dirToL = lv.vToL / float(lv.distToL);
	return lv;
}
inline float Attenuate(float attConst, float attLin, float attQuad, float distFragToL)
{
	return 1.0f / (attConst + attLin * distFragToL + attQuad * (distFragToL * distFragToL));
}
inline float3 Diffuse(
	float3U diffuseColor,
	float diffuseIntensity,
	float att,
	const float3U viewDirFragToL,
	const float3U viewNormal)
{
	using namespace dx;
	return diffuseColor * diffuseIntensity * att * dx::XMVectorMax(dx::XMVectorZero(), dx::XMVector3Dot(viewDirFragToL, viewNormal));
}

inline float3 Speculate(
	float3 specularColor,
	float specularIntensity,
	const float3U viewNormal,
	const float3U viewFragToL,
	const float3U viewPos,
	float att,
	float specularPower)
{
	using namespace dx;
	// calculate reflected light vector
	const float3 w = viewNormal * dx::XMVector3Dot(viewFragToL, viewNormal);
	const float3 r = dx::XMVector3Normalize(w * 2.0f - viewFragToL);
	// vector from camera to fragment (in view space)
	const float3 viewCamToFrag = dx::XMVector3Normalize(viewPos);
	// calculate specular component color based on angle between
	// viewing vector and reflection vector, narrow with power function
	return att * specularColor * specularIntensity * dx::XMVectorPow(dx::XMVectorMax(dx::XMVectorZero(), dx::XMVector3Dot(-r, viewCamToFrag)), dx::XMVectorReplicate(specularPower));
}

class SimplePS : public PixelShader<SimplePS>
{
public:
	struct PointLightCBuf
	{
		float3U viewLightPos;
		float3U ambient;
		float3U diffuseColor;
		float1U diffuseIntensity;
		float1U attConst;
		float1U attLin;
		float1U attQuad;
	}light;
	struct CBuffer
	{
		float3U materialColor;
		float3U specularColor;
		float1U specularWeight;
		float1U specularGloss;
	}cbuf;
public:
	float4 main(float3 viewFragPos, float3 viewNormal)
	{
		using namespace dx;
		// normalize the mesh normal
		viewNormal = dx::XMVector3Normalize(viewNormal);	
		// fragment to light vector data
		const LightVectorData lv = CalculateLightVectorData(light.viewLightPos, viewFragPos);
		// attenuation
		const float att = Attenuate(light.attConst, light.attLin, light.attQuad, lv.distToL);
		// diffuse
		const float3 diffuse = Diffuse(light.diffuseColor, light.diffuseIntensity, att, lv.dirToL, viewNormal);
		//// specular
		const float3 specular = Speculate(
			light.diffuseColor * float(light.diffuseIntensity) * cbuf.specularColor, cbuf.specularWeight, viewNormal,
			lv.vToL, viewFragPos, att, cbuf.specularGloss
		);
		// final color
		return dx::XMVectorSetW(dx::XMVectorSaturate((diffuse + light.ambient) * cbuf.materialColor + specular), 1.0f);
	}
public:
	void UpdateConstants(void* const* constants)override
	{
		cbuf = constants[0] ? *static_cast<const CBuffer*>(constants[0]) : CBuffer{};
		light = constants[1] ? *static_cast<const PointLightCBuf*>(constants[1]) : PointLightCBuf{};
	};
};