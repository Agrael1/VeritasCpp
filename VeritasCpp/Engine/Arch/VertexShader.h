#pragma once
#include <string_view>
#include "Bindable.h"
#include "../ShaderTech/ShaderCommon.h"

class VertexShader : public Bindable
{
public:
	VertexShader(std::string_view shadertag) 
	{
		VFMakeShader(shadertag.data(), &vs);
	}
public:
	void Bind(class VeritasEngine& in)override
	{
		GetContext(in)->VSSetShader(vs.Get());
	}
private:
	wrl::ComPtr<IVVertexShader> vs;
};

