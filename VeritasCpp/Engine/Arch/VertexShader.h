#pragma once
#include <string_view>
#include "Bindable.h"
#include "../ShaderTech/ShaderCommon.h"

class VertexShader : public Bindable
{
public:
	VertexShader(std::string_view shadertag);
public:
	void Bind(class VeritasEngine& in)override;
private:
	wrl::ComPtr<IVShader> vs;
};

