#pragma once
#include "XMHelpers.h"

class PC3Transformer
{
public:
	PC3Transformer(uint16_t ScreenWidth, uint16_t ScreenHeight)
		:
		xFactor(float(ScreenWidth) / 2.0f),
		yFactor(float(ScreenHeight) / 2.0f)
	{}
	vec3A& Transform(vec3A& v) const
	{
		const float zInv = 1.0f /*/ v.z*/;
		v.x = (v.x * zInv + 1.0f) * xFactor;
		v.y = (-v.y * zInv + 1.0f) * yFactor;
		return v;
	}
	vec3A GetTransformed(const vec3A& v) const
	{
		return Transform(vec3A(v));
	}
private:
	float xFactor;
	float yFactor;
};