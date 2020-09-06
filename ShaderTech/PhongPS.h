#pragma once
#include "Shader.h"

struct LightVectorData
{
	float3 vToL;
	float3 dirToL;
	float1 distToL;
};

inline LightVectorData CalculateLightVectorData(const float3 lightPos, const float3 fragPos)
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
	float3 diffuseColor,
	float diffuseIntensity,
	float att,
	const float3 viewDirFragToL,
	const float3 viewNormal)
{
	using namespace dx;
	return diffuseColor * diffuseIntensity * att * dx::XMVectorMax(dx::XMVectorZero(), dx::XMVector3Dot(viewDirFragToL, viewNormal));
}

inline float3 Speculate(
	float3 specularColor,
	float specularIntensity,
	const float3 viewNormal,
	const float3 viewFragToL,
	const float3 viewPos,
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


class PhongPS : public PixelShader<PhongPS>
{
public:
	struct PointLightCBuf
	{
		float3 viewLightPos;
		float3 ambient;
		float3 diffuseColor;
		float1 diffuseIntensity;
		float1 attConst;
		float1 attLin;
		float1 attQuad;
	}light;
	struct CBuffer
	{
		float3 materialColor;
		float3 specularColor;
		float1 specularWeight;
		float1 specularGloss;
	}cbuf;
public:
	float4A main(float3 viewFragPos, float3 viewNormal)
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
	void __stdcall UpdateConstants(void* const* constants)override
	{
		cbuf = constants[0] ? *static_cast<const CBuffer*>(constants[0]) : CBuffer{};
		light = constants[1] ? *static_cast<const PointLightCBuf*>(constants[1]) : PointLightCBuf{};
	};
};