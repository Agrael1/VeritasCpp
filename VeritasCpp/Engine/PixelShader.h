#pragma once
#include "Bindable.h"
#include <string_view>

class PixelShader : public Bindable
{
public:
	PixelShader(std::string_view shaderName);
public:
	void Bind(VeritasEngine& vin)override;
private:
	wrl::ComPtr<IVShader> pPS;
};